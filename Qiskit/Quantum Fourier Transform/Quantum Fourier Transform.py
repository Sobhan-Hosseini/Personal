"""
  Quantum Fourier Transform
  Author: Seyed Sobhan Hosseini
  Date: Summer 2024
"""

from qiskit import QuantumCircuit, Aer, transpile, assemble

# Function to create a QFT circuit
def qft(n):
    qc = QuantumCircuit(n)
    for i in range(n):
        qc.h(i)
        for j in range(i + 1, n):
            qc.cp(np.pi / 2**(j - i), i, j)
    qc.reverse_bits()
    return qc

# Create a 3-qubit QFT circuit
qc = qft(3)

# Draw the circuit
qc.draw('mpl')

# Simulate on the statevector simulator
simulator = Aer.get_backend('statevector_simulator')
tqc = transpile(qc, simulator)
qobj = assemble(tqc)
result = simulator.run(qobj).result()

# Get the final statevector
statevector = result.get_statevector()
print("Statevector after QFT:", statevector)
