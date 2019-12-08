# -*- coding:utf-8 -*-
# created by zhutao chu
import cv2
import os
import numpy as np
from matplotlib import pyplot as plt

def getrow(img):
    row_num = []
    img_w = img.shape[1]
    for i in range(img_w):
        row_num.append(len(np.where(img[:,i]==0)[0]))
    return row_num

def getcol(img):
    col_num = []
    img_h = img.shape[0]
    for i in range(img_h):
        col_num.append(len(np.where(img[i,:]==0)[0]))
    return col_num

def getpoint(row_num,col_num):
    for i in range(1,len(row_num)):
        if row_num[i]>row_num[i-1]:
            left = i
            break
    for i in range(1,len(col_num)):
        if col_num[i]>col_num[i-1]:
            top = i
            break
    for i in range(len(row_num)-1,1,-1):
        if row_num[i]<row_num[i-1]:
            right = i-1
            break
    for i in range(len(col_num)-1,1,-1):
        if col_num[i]<col_num[i-1]:
            bottom = i-1
            break
    return left,top,right,bottom

data = os.listdir('./input')
data.sort()

labels = os.listdir('./data')
labels.sort()
label = []

for file in labels:
    img_name = os.path.join('./data',file)
    image = cv2.imread(img_name)
    img = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    label.append(img)

for file in data:
    f_word = ''
    img_name = os.path.join('./input',file)
    image = cv2.imread(img_name)
    img = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
    row_num = getrow(img)
    col_num = getcol(img)
    left, top, right, bottom = getpoint(row_num, col_num)
    img_val = img[top:bottom,left:right]
    if len(np.where(img_val[:,1]==0)[0])==7:
        left = left -1
    # cv2.namedWindow('img2', 0)
    # cv2.imshow('img2', img_val)
    # cv2.waitKey()

    w = int((right-left+1)/6)+1
    x = left
    y = top
    for i in range(w):
        image1 = img[y:y+8,x:x+5]
        x =x+6
        for j in range(len(label)):
            res = cv2.matchTemplate(image1, label[j], cv2.TM_CCOEFF_NORMED)
            # difference = image1-label[j]#cv2.subtract(image1, label[j])
            # result = not np.any(difference)
            if res[0][0]>0.9:
                if j ==10:
                    f_word = f_word + '.'
                else:
                    f_word = f_word + str(j)
                # break

            # cv2.namedWindow('img1',0)
            # cv2.namedWindow('img2',0)
            # cv2.namedWindow('img3',0)
            # cv2.imshow('img1',image1)
            # cv2.imshow('img2',label[j])
            # # cv2.imshow('img3',difference)
            # cv2.waitKey()
    font = cv2.FONT_HERSHEY_SIMPLEX

    cv2.putText(image,f_word,(40,40), font,0.5, (0,0,255), 1)
    print(f_word)
    cv2.namedWindow('img1',0)
    cv2.imshow('img1',image)


    cv2.waitKey()
