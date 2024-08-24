"""
  Quantum Phase Estimation
  Author: Seyed Sobhan Hosseini
  Date: Summer 2024
"""

import numpy as np
from qiskit import QuantumCircuit, Aer, execute
from qiskit.visualization import plot_histogram

# Define the phase estimation circuit (2 qubits for estimation, 1 for the unitary)
qc = QuantumCircuit(3, 1)  

# Apply Hadamard gates to the first two qubits
qc.h([0, 1])

# Controlled-U operations
qc.cp(np.pi/4, 0, 2)
qc.cp(np.pi/2, 1, 2)

# Apply inverse QFT to the first two qubits
qc.h(1)
qc.cp(-np.pi/2, 0, 1)
qc.h(0)

# Measure the first qubit
qc.measure(0, 0)

# Draw the circuit
qc.draw('mpl')

# Simulate the circuit
simulator = Aer.get_backend('qasm_simulator')
result = execute(qc, simulator, shots=1024).result()

# Get and plot the results
counts = result.get_counts()
plot_histogram(counts)
