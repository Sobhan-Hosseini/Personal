"""
  Basic Quantum Circuit
  Author: Seyed Sobhan Hosseini
  Date: Summer 2024
"""

from qiskit import QuantumCircuit, Aer, execute

# Create a quantum circuit with 1 qubit and 1 classical bit
qc = QuantumCircuit(1, 1)

# Apply a Hadamard gate
qc.h(0)

# Measure the qubit into the classical bit
qc.measure(0, 0)

# Draw the circuit
qc.draw('mpl')

# Aer simulator
simulator = Aer.get_backend('qasm_simulator')

# Execute the circuit on the qasm simulator
job = execute(qc, simulator, shots=1024)

# Get the results from the job
result = job.result()

# Get the counts of the results
counts = result.get_counts(qc)
print("\nTotal count for 0 and 1 are:", counts)

