"""
  SVM
  Author: Seyed Sobhan Hosseini
  Date: Friday, June 28, 2024, 10:01:54 PM
"""

import time
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.svm import SVC
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix

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

# Standardize the data
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# SVM**************************************************
print("\nSupport Vector Machines Classifier:")
# Initializing the Support Vector Machines classifier
svm_classifier = SVC()

# Train the model on the training data
start_time = time.time()
svm_classifier.fit(X_train, y_train)
svm_training_time = time.time() - start_time
print("training time : ")
print(svm_training_time)

# Making predictions on the testing data
start_time = time.time()
y_pred_svm = svm_classifier.predict(X_test)
svm_prediction_time = time.time() - start_time
print("prediction time : ")
print(svm_prediction_time)

print("Predictions:", y_pred_svm[:10])

# Calculate accuracy
accuracy_svm = accuracy_score(y_test, y_pred_svm)
print("Accuracy:", accuracy_svm)

#Calculate precision
precision_svm = precision_score(y_test, y_pred_svm, average='binary', pos_label=1)
print("Precision:", precision_svm)

#Calculate recall
recall_svm = recall_score(y_test, y_pred_svm, average='binary', pos_label=1)
print("recall:", recall_svm)

#Calculate f1
f1_svm = f1_score(y_test, y_pred_svm, average='binary', pos_label=1)
print("f1:", f1_svm)

# Generate confusion matrix
conf_matrix_svm = confusion_matrix(y_test, y_pred_svm)
print("Confusion matrix:", conf_matrix_svm)

# Plot confusion matrix
plt.figure(figsize=(8, 6))
sns.heatmap(conf_matrix_svm, annot=True, fmt='d', cmap='Blues')
plt.xlabel('Predicted')
plt.ylabel('Actual')
plt.title('Confusion Matrix')
plt.show()
