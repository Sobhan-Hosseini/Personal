"""
  Shor's Algorithm
  Author: Seyed Sobhan Hosseini
  Date: Summer 2024
"""

from qiskit.algorithms import Shor
from qiskit import Aer
from qiskit.utils import QuantumInstance

# Initialize Shor's algorithm to factorize 10
shor = Shor()

# Run on a simulator
quantum_instance = QuantumInstance(backend=Aer.get_backend('qasm_simulator'))
result = shor.factor(N=10, quantum_instance=quantum_instance)

# Output the factors
print("Factors of 10:", result.factors)
