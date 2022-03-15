#Махмудов Орхан группа М8О-305Б-18
#Поверхность Кунса (образующие кубические кривые Безье)

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from tkinter import Tk, Label, Button, Entry

def bezier (x1,y1,x2,y2,x3,y3,x4,y4):
    t = np.arange(0, 1, 0.001)
    x = np.zeros(len(t))
    y = np.zeros(len(t))
    for i in range (len(t)):
        x[i] =  (1 - t[i])**3 * t[i] * x1 + 3 * (1 - t[i])**2 * t[i] * x2 + 3 * (1 - t[i]) * t[i]**2 * x3 + t[i]**3 * x4
        y[i] =  (1 - t[i])**3 * t[i] * y1 + 3 * (1 - t[i])**2 * t[i] * y2 + 3 * (1 - t[i]) * t[i]**2 * y3 + t[i]**3 * y4
    return x, y

def clicked():

    n = int(txt1.get())
    x1,y1,x2,y2,x3,y3,x4,y4 = 0, 0, 0.3, 1, 0.6, -2, 1, 0

    t = np.arange(0, 1, 0.001)
    z0 = np.zeros(len(t))
    z1 = np.ones(len(t))
    x_a, y_a = bezier(x1,y1,x2,y2,x3,y3,x4,y4)
    fig = plt.figure("Поверхность Кунса (образующие кубические кривые Безье)")
    ax = fig.gca(projection='3d')

    i = 0
    while i < (len(t) - (1000//n)):
        i = i + (1000//n)
        pr = z0 + x_a[i]
        yt = y_a + y_a[i]
        ax.plot(pr, yt, x_a, color = 'purple')

    i = 0
    # while i < (len(t) - (1000//n)):
    #     i = i + (1000//n)
    #     pr = z0 + x_a[i]
    #     yt = y_a + y_a[i]
    #     ax.plot(x_a, yt, pr, color = 'pink')

    ax.plot(x_a,y_a,z0, color = 'black')
    ax.plot(x_a,y_a,z1, color = 'black')
    ax.plot(z0,y_a,x_a, color = 'black')
    ax.plot(z1,y_a,x_a, color = 'black')

    plt.show()

window = Tk()
window.title("Поверхность Кунса (образующие кубические кривые Безье)")
window.geometry("300x100")
lb2 = Label(window, text = "Введите количество разбиений от 1 до 1000: ")  
lb2.place(x = 1, y = 10)
txt1 = Entry(window, width = 4)
txt1.place(x = 250, y = 10)

btn = Button(window, text = "Запуск", bg = "green", fg = "white", command = clicked)
btn.place(x = 150, y = 50)
window.mainloop()