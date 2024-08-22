"""
  NaiveBayes
  Author: Seyed Sobhan Hosseini
  Date: sFriday, June 28, 2024, 10:01:54 PM
"""

import time
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.naive_bayes import GaussianNB
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

# Separating features and target variable
X = adult_data_encoded.drop('income', axis=1) 
y = adult_data_encoded['income']  

# Encoding target variable
y = y.apply(lambda x: 1 if x == ' >50K' else 0)

# NAIVE BAYES**************************************************
print("\nNaive Bayes Classifier:")
# Initializing the Naive Bayes classifier
nb_classifier = GaussianNB()

# k-Fold Cross-Validation
kf = KFold(n_splits=5, shuffle=True, random_state=42)

cv_results_nb = cross_val_score(nb_classifier, X, y, cv=kf, scoring='accuracy')

print("Cross-Validation Accuracy Scores:", cv_results_nb)
print("Mean Cross-Validation Accuracy:", np.mean(cv_results_nb))
print("Standard Deviation of Cross-Validation Accuracy:", np.std(cv_results_nb))

# 80% trainSet, 20% testSet
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Train the model on the training data
start_time = time.time()
nb_classifier.fit(X_train, y_train)
nb_training_time = time.time() - start_time
print("Training time:", nb_training_time)

# Making predictions on the testing data
start_time = time.time()
y_pred_nb = nb_classifier.predict(X_test)
nb_prediction_time = time.time() - start_time
print("Prediction time:", nb_prediction_time)

print("Predictions:", y_pred_nb[:10])

# Calculate accuracy
accuracy_nb = accuracy_score(y_test, y_pred_nb)
print("Accuracy:", accuracy_nb)

# Calculate precision
precision_nb = precision_score(y_test, y_pred_nb)
print("Precision:", precision_nb)

# Calculate recall
recall_nb = recall_score(y_test, y_pred_nb)
print("Recall:", recall_nb)

# Calculate F1 score
f1_nb = f1_score(y_test, y_pred_nb)
print("F1 Score:", f1_nb)

# Generate confusion matrix
conf_matrix_nb = confusion_matrix(y_test, y_pred_nb)
print("Confusion Matrix:\n", conf_matrix_nb)

# showing Classification report
print("\nClassification Report:\n", classification_report(y_test, y_pred_nb))

# Plot confusion matrix
plt.figure(figsize=(8, 6))
sns.heatmap(conf_matrix_nb, annot=True, fmt='d', cmap='Blues')
plt.xlabel('Predicted')
plt.ylabel('Actual')
plt.title('Confusion Matrix')
plt.show()

# ROC Curve and AUC
y_proba_nb = nb_classifier.predict_proba(X_test)[:, 1]
fpr_nb, tpr_nb, thresholds_nb = roc_curve(y_test, y_proba_nb)
roc_auc_nb = roc_auc_score(y_test, y_proba_nb)

plt.figure()
plt.plot(fpr_nb, tpr_nb, color='darkorange', lw=2, label='ROC curve (area = %0.2f)' % roc_auc_nb)
plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')
plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver Operating Characteristic (ROC) Curve')
plt.legend(loc="lower right")
plt.show()