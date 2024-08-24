"""
  Grover's Algorithm
  Author: Seyed Sobhan Hosseini
  Date: Summer 2024
"""

from qiskit import QuantumCircuit, Aer, transpile, assemble
from qiskit.visualization import plot_histogram

# Create a quantum circuit with 2 qubits and 2 classical bits
qc = QuantumCircuit(2, 2)

# Initialize the qubits in superposition
qc.h([0, 1])

# Oracle (|11⟩ state)
qc.cz(0, 1)

# Apply the Grover diffusion operator
qc.h([0, 1])
qc.z([0, 1])
qc.cz(0, 1)
qc.h([0, 1])

# Measure the qubits
qc.measure([0, 1], [0, 1])

# Draw the circuit
qc.draw('mpl')

# Simulate the circuit
sim = Aer.get_backend('qasm_simulator')
t_qc = transpile(qc, sim)
qobj = assemble(t_qc)
result = sim.run(qobj).result()

# Get and plot results
counts = result.get_counts(qc)
plot_histogram(counts)
