"""
  NeuralNetwork
  Author: Seyed Sobhan Hosseini
  Date: sFriday, June 28, 2024, 10:01:54 PM
"""

import time
import numpy as np
import pandas as pd
import seaborn as sns
import tensorflow as tf
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler, LabelEncoder
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix, roc_curve, roc_auc_score

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
y = y.apply(lambda x: 1 if x == ' >50K' else 0)

# Spliting dataSet into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Standardize the input features
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# Encoding the target variable
label_encoder = LabelEncoder()
y_train_encoded = label_encoder.fit_transform(y_train)
y_test_encoded = label_encoder.transform(y_test)

# Converting encoded labels to one-hot encoded format
y_train_one_hot = tf.keras.utils.to_categorical(y_train_encoded)
y_test_one_hot = tf.keras.utils.to_categorical(y_test_encoded)

#NEURAL NETWORK**************************************************
print("\nNeural Network:")
# Defining the Neural Network model
model = tf.keras.Sequential([
    tf.keras.layers.Dense(128, activation='relu', input_shape=(X_train_scaled.shape[1],)),
    tf.keras.layers.Dense(32, activation='relu'),
    tf.keras.layers.Dense(64, activation='relu'),
    tf.keras.layers.Dense(16, activation='relu'),
    tf.keras.layers.Dense(2, activation='sigmoid')
])

# Compiling the model
model.compile(optimizer='RMSprop', loss='binary_crossentropy', metrics=['accuracy'])

# Train the model
start_time = time.time()
history = model.fit(X_train_scaled, y_train_one_hot, epochs=10, batch_size=32, validation_split=0.2, verbose=1)
nn_training_time = time.time() - start_time
print("Training Time:", nn_training_time)

# testing data
test_loss, test_accuracy = model.evaluate(X_test_scaled, y_test_one_hot, verbose=0)
print("Test Accuracy:", test_accuracy)

# Making predictions on the testing data
start_time = time.time()
y_pred_prob = model.predict(X_test_scaled)
nn_prediction_time = time.time() - start_time
print("Prediction Time:", nn_prediction_time)

# Converting predictions to class labels
y_pred = np.argmax(y_pred_prob, axis=1)
y_test = np.argmax(y_test_one_hot, axis=1)

# Calculate metrics: Accuracy, Precision, Recall, F1-score
accuracy_nn = accuracy_score(y_test, y_pred)
precision_nn = precision_score(y_test, y_pred)
recall_nn = recall_score(y_test, y_pred)
f1_nn = f1_score(y_test, y_pred)
print("\nMetrics:")
print("Accuracy:", accuracy_nn)
print("Precision:", precision_nn)
print("Recall:", recall_nn)
print("F1 Score:", f1_nn)

# Generate confusion matrix
conf_matrix_nn = confusion_matrix(y_test, y_pred)
print("\nConfusion Matrix:")
print(conf_matrix_nn)

# Plot confusion matrix
plt.figure(figsize=(8, 6))
sns.heatmap(conf_matrix_nn, annot=True, fmt='d', cmap='Blues')
plt.xlabel('Predicted')
plt.ylabel('Actual')
plt.title('Confusion Matrix')
plt.show()

# ROC curve and AUC
fpr, tpr, thresholds = roc_curve(y_test, y_pred_prob[:, 1])
roc_auc = roc_auc_score(y_test, y_pred_prob[:, 1])

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