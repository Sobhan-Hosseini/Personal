"""
  Signals & Systems Q4
  Author: Seyed Sobhan Hosseini
  Date: Sunday, June 25, 2023, 12:59:09 PM
"""

import numpy as np
import matplotlib.pyplot as plt

# Define x(n) and y(n)
n = np.arange(101)
x = np.cos(n*np.pi/2)
y = np.exp(1j*n*np.pi/4) * x

# fourier transform of x(n)
N = len(x)
X = np.zeros(N, dtype=np.complex128)
T = 2*np.pi
dt = T/N
t = np.linspace(0, T-dt, N)
for k in range(N):
    X[k] = dt * np.sum(x * np.exp(-1j*t*k))

    

# fourier transform of y(n)
Y = np.zeros(N, dtype=np.complex128)
for k in range(N):
    Y[k] = dt * np.sum(y * np.exp(-1j*t*k))


# Plot x(n)
freq = np.arange(N) * (2*np.pi / T)
plt.figure()
plt.plot(freq, np.abs(X))
plt.title("x(n)")
plt.xlabel("Frequency")
plt.ylabel("X")



# Plot y(n)
plt.figure()
plt.plot(freq, np.abs(Y))
plt.title("y(n)")
plt.xlabel("Frequency")
plt.ylabel("Y")

plt.show()



import numpy as np
import matplotlib.pyplot as plt

a = [1, -0.8]
b = [1]

# Define x(n)
n = np.arange(0, 700)
x = np.cos(np.pi * n * 0.05) * (n >= 0)

# y(n) 
y = np.zeros_like(x)
y[0] = b[0] * x[0]
for i in range(1, len(n)):
    y[i] = b[0] * x[i] - a[1] * y[i-1]  

# Plot 
plt.plot(n, x, label='Input')
plt.plot(n, y, label='Output')
plt.xlabel('n')
plt.legend()
plt.show()

import numpy as np
import matplotlib.pyplot as plt

# h(n)
def h(n):
    return 0.9**np.abs(n)

# x(n)
def x(n):
    return (0.6**np.abs(n))*(np.heaviside(n+10,1)-np.heaviside(n-11,1))

#  H(e^(jω))
ω = np.linspace(-np.pi,np.pi,1000)
H = np.zeros_like(ω,dtype=np.complex)
for i in range(len(ω)):
    z = np.exp(1j*ω[i])
    H[i] = np.sum(h(np.arange(-50,51))*z**(-np.arange(-50,51)))

# Calculate y(n)
n = np.arange(-50,51)
y = np.convolve(x(n),h(n),mode='same')

# Plot H(e^(jw))
plt.figure()
plt.plot(ω,np.abs(H))
plt.title('|H(e^(jω))|')
plt.xlabel('ω')

# Plot y(n)
plt.figure()
plt.stem(n,y)
plt.title('y(n)')
plt.xlabel('n')
plt.show()