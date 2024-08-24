"""
  Quantum Teleportation Protocol
  Author: Seyed Sobhan Hosseini
  Date: Summer 2024
"""

from qiskit import QuantumCircuit, Aer, transpile, assemble
from qiskit.visualization import plot_histogram

# Create a Quantum Circuit with 3 qubits and 3 classical bits
qc = QuantumCircuit(3, 3)

# Create Bell pair
qc.h(1)
qc.cx(1, 2)

# Alice prepares a state to teleport
qc.x(0)  

# Alice applies CNOT and H gates
qc.cx(0, 1)
qc.h(0)

# Alice measures her qubits
qc.measure([0, 1], [0, 1])

# Bob applies X and Z gates conditionally
qc.cx(1, 2)
qc.cz(0, 2)

# Measure Bob's qubit
qc.measure(2, 2)

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
