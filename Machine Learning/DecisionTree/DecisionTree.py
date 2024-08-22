"""
  DecisionTree
  Author: Seyed Sobhan Hosseini
  Date: Friday, June 28, 2024, 10:01:54 PM
"""

import time
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import train_test_split, cross_val_score, KFold
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix, classification_report, roc_auc_score, roc_curve

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

# 80% trainSet, 20% testSet
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# DECISION TREE**************************************************
print("\nDecision Tree Classifier:")
# Initializing the Decision Tree classifier
dt_classifier = DecisionTreeClassifier(random_state=42)

# k-Fold Cross-Validation
kf = KFold(n_splits=5, shuffle=True, random_state=42)
cv_results_dt = cross_val_score(dt_classifier, X, y, cv=kf, scoring='accuracy')

print("Cross-Validation Accuracy Scores:", cv_results_dt)
print("Mean Cross-Validation Accuracy:", np.mean(cv_results_dt))
print("Standard Deviation of Cross-Validation Accuracy:", np.std(cv_results_dt))

# Train the classifier on the training data
start_time = time.time()
dt_classifier.fit(X_train, y_train)
dt_training_time = time.time() - start_time
print("Training time:", dt_training_time)

# Making predictions on the testing data
start_time = time.time()
y_pred_dt = dt_classifier.predict(X_test)
dt_prediction_time = time.time() - start_time
print("Prediction time:", dt_prediction_time)

print("Predictions:", y_pred_dt[:10])

# Calculate accuracy
accuracy_dt = accuracy_score(y_test, y_pred_dt)
print("Accuracy:", accuracy_dt)

# Calculate precision
precision_dt = precision_score(y_test, y_pred_dt)
print("Precision:", precision_dt)

# Calculate recall
recall_dt = recall_score(y_test, y_pred_dt)
print("Recall:", recall_dt)

# Calculate F1 score
f1_dt = f1_score(y_test, y_pred_dt)
print("F1 Score:", f1_dt)

# Generate confusion matrix
conf_matrix_dt = confusion_matrix(y_test, y_pred_dt)
print("Confusion Matrix:\n", conf_matrix_dt)

# showing Classification report
print("\nClassification Report:\n", classification_report(y_test, y_pred_dt))

# Plot confusion matrix
plt.figure(figsize=(8, 6))
sns.heatmap(conf_matrix_dt, annot=True, fmt='d', cmap='Blues')
plt.xlabel('Predicted')
plt.ylabel('Actual')
plt.title('Confusion Matrix')
plt.show()

# ROC Curve and AUC
y_proba_dt = dt_classifier.predict_proba(X_test)[:, 1]
fpr_dt, tpr_dt, thresholds_dt = roc_curve(y_test, y_proba_dt)
roc_auc_dt = roc_auc_score(y_test, y_proba_dt)

plt.figure()
plt.plot(fpr_dt, tpr_dt, color='darkorange', lw=2, label='ROC curve (area = %0.2f)' % roc_auc_dt)
plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')
plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver Operating Characteristic (ROC) Curve')
plt.legend(loc="lower right")
plt.show()
