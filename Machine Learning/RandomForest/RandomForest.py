"""
  RandomForest
  Author: Seyed Sobhan Hosseini
  Date: sFriday, June 28, 2024, 10:01:54 PM
"""

import time
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split, cross_val_score, KFold
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix, roc_auc_score, roc_curve, classification_report

file_path = r"C:\Users\Sobhan\Desktop\ML\Project\adult_database\adult.data"

columns = ["age", "workclass", "fnlwgt", "education", "education-num", "marital-status", "occupation", "relationship", "race", "sex", "capital-gain", "capital-loss", "hours-per-week", "native-country", "income"]
adult_data = pd.read_csv(file_path, names=columns, na_values=" ?")

print("Shape of the raw dataset:", adult_data.shape)

print(adult_data.head())

# Removing rows with missing values
adult_data_cleaned = adult_data.dropna()

print("Shape of the cleaned dataset:", adult_data_cleaned.shape)

# Encoding categorical variables using one-hot encoding
adult_data_encoded = pd.get_dummies(adult_data_cleaned, columns=["workclass", "education", "marital-status", "occupation", "relationship", "race", "sex", "native-country"])

print("Encoded dataset:")
print(adult_data_encoded.head())

# Separating features (X) and target variable (y)
X = adult_data_encoded.drop('income', axis=1) 
y = adult_data_encoded['income'] 

# Encoding target variable
y = y.apply(lambda x: 1 if x == ' >50K' else 0)

# Spliting the dataset into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# RANDOM FOREST**************************************************
print("\nRandom Forest Classifier:")
# Initializing the Random Forest classifier 
rf_classifier = RandomForestClassifier(random_state=42, n_estimators=100, max_features='sqrt', max_depth=25, min_samples_split=5, min_samples_leaf=2)

# k-Fold Cross-Validation
kf = KFold(n_splits=5, shuffle=True, random_state=42)
cv_results = cross_val_score(rf_classifier, X, y, cv=kf, scoring='accuracy')

print("Cross-Validation Accuracy Scores:", cv_results)
print("Mean Cross-Validation Accuracy:", np.mean(cv_results))
print("Standard Deviation of Cross-Validation Accuracy:", np.std(cv_results))

# Train the classifier on the training data
start_time = time.time()
rf_classifier.fit(X_train, y_train)
rf_training_time = time.time() - start_time
print("Training time:", rf_training_time)

# Make predictions on the testing data
start_time = time.time()
y_pred_rf = rf_classifier.predict(X_test)
rf_prediction_time = time.time() - start_time
print("Prediction time:", rf_prediction_time)

print("Predictions:", y_pred_rf[:10])

# Calculate accuracy
accuracy_rf = accuracy_score(y_test, y_pred_rf)
print("Accuracy:", accuracy_rf)

# Calculate precision
precision_rf = precision_score(y_test, y_pred_rf)
print("Precision:", precision_rf)

# Calculate recall
recall_rf = recall_score(y_test, y_pred_rf)
print("Recall:", recall_rf)

# Calculate F1 score
f1_rf = f1_score(y_test, y_pred_rf)
print("F1 Score:", f1_rf)

# Generate confusion matrix
conf_matrix_rf = confusion_matrix(y_test, y_pred_rf)
print("Confusion Matrix:\n", conf_matrix_rf)

# showing Classification report
print("\nClassification Report:\n", classification_report(y_test, y_pred_rf))

# Plot confusion matrix
plt.figure(figsize=(8, 6))
sns.heatmap(conf_matrix_rf, annot=True, fmt='d', cmap='Blues')
plt.xlabel('Predicted')
plt.ylabel('Actual')
plt.title('Confusion Matrix')
plt.show()

# ROC Curve and AUC
y_proba_rf = rf_classifier.predict_proba(X_test)[:, 1]
fpr, tpr, thresholds = roc_curve(y_test, y_proba_rf)
roc_auc = roc_auc_score(y_test, y_proba_rf)

plt.figure()
plt.plot(fpr, tpr, color='darkorange', lw=2, label='ROC curve (area = %0.2f)' % roc_auc)
plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')
plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver Operating Characteristic (ROC) Curve')
plt.legend(loc="lower right")
plt.show()