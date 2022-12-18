import matplotlib.pyplot as plt
from matplotlib.ticker import (AutoMinorLocator, MultipleLocator)
import numpy as np

data = open('C:/dev/CLionProjects/3sem/HH_model/cmake-build-debug/data.txt', 'r')
neur = []
curr = []
flag = True
for line in data:
    if line == "\n":
        flag = False
    if flag:
        neur.append(line.split())
    else:
        if line !="\n":
            curr.append(line.split())


t = []
h = []
m = []
n = []
v = []

for i in range(len(neur)):

    t.append(float(neur[i][0]))
    h.append(float(neur[i][1]))
    m.append(float(neur[i][2]))
    n.append(float(neur[i][3]))
    v.append(float(neur[i][4]))

tc = []
I = []
for i in range(len(curr)):
    tc.append(float(curr[i][0]))
    I.append(float(curr[i][1]))






# Диапазон осей

t_LIM_BOTTOM = 0
v_LIM_BOTTOM = -0.02
x_LIM_BOTTOM = 0
i_LIM_BOTTOM = 0

t_LIM_UP = t[len(t)-1]+1
v_LIM_UP = 0.12
x_LIM_UP = 1
i_LIM_UP = 0.000015


fig, (ax0, ax1, ax2) = plt.subplots(3,1)

# Диапазоны осей
ax0.set_xlim(t_LIM_BOTTOM, t_LIM_UP)
ax0.set_ylim(v_LIM_BOTTOM, v_LIM_UP)

ax1.set_xlim(t_LIM_BOTTOM, t_LIM_UP)
ax1.set_ylim(x_LIM_BOTTOM, x_LIM_UP)

ax2.set_xlim(t_LIM_BOTTOM, t_LIM_UP)
ax2.set_ylim(i_LIM_BOTTOM, i_LIM_UP)

# Шаг сетки для главных делений
ax0.xaxis.set_major_locator(MultipleLocator(10))
ax0.yaxis.set_major_locator(MultipleLocator(0.04))

ax1.xaxis.set_major_locator(MultipleLocator(10))
ax1.yaxis.set_major_locator(MultipleLocator(0.2))

ax2.xaxis.set_major_locator(MultipleLocator(10))
ax2.yaxis.set_major_locator(MultipleLocator(0.000002))


# Количество промежутков между главными делениями
ax0.xaxis.set_minor_locator(AutoMinorLocator(5))
ax0.yaxis.set_minor_locator(AutoMinorLocator(5))

ax1.xaxis.set_minor_locator(AutoMinorLocator(5))
ax1.yaxis.set_minor_locator(AutoMinorLocator(5))

ax2.xaxis.set_minor_locator(AutoMinorLocator(5))
ax2.yaxis.set_minor_locator(AutoMinorLocator(5))

# Сетка для главных и дополнительных делений
ax0.grid(which='major', color='#CCCCCC', linestyle='--')
ax0.grid(which='minor', color='#CCCCCC', linestyle=':')

ax1.grid(which='major', color='#CCCCCC', linestyle='--')
ax1.grid(which='minor', color='#CCCCCC', linestyle=':')

ax2.grid(which='major', color='#CCCCCC', linestyle='--')
ax2.grid(which='minor', color='#CCCCCC', linestyle=':')


# Подписи осей
plt.xlabel('t, ms')
ax0.set_ylabel('v, mV')
ax2.set_ylabel('I_inp, microA')

# Название графика
#ax0.title('Зависимость v(t)')

ax0.plot(t, v)

ax1.plot(t, h, label = "h")
ax1.plot(t, m, label = "m")
ax1.plot(t, n, label = "n")
ax2.plot(tc, I)

ax1.legend()
plt.show()

