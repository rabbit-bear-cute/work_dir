#!/usr/bin/env python
#coding=utf-8

from numpy import *
import matplotlib.pyplot
matplotlib.pyplot.switch_backend('agg')
import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

def loadData(file):
    dataMat = []
    labelMat = []
    fp = open(file)
    for line in fp.readlines():
        lineArr = line.strip().split()
        dataMat.append([float(lineArr[0]), float(lineArr[1])])
        labelMat.append(float(lineArr[2]))
    return dataMat,labelMat

def selectJrand(i,m):
	j=i
	while(j==i):
		j=int(random.uniform(0,m))
	return j

def clipAlpha(aj,H,L):
	if aj > H:
		aj = H
	if L > aj:
		aj = L
	return aj

def smoSimple(dataMatIn, classLabels, c, toler, maxIter):
	dataMatrix = mat(dataMatIn); labelMat = mat(classLabels).transpose()
	b = 0; m,n = shape(dataMatrix)
	alphas = mat(zeros((m,1)))
	iter = 0
	while(iter < maxIter):
		alphaPairsChanged = 0
		for i in range(m):
			fXi = float(multiply(alphas,labelMat).T*(dataMatrix*dataMatrix[i,:].T)) + b
			Ei = fXi - float(labelMat[i])
			if((labelMat[i]*Ei < -toler) and (alphas[i] < c)) or ((labelMat[i]*Ei > toler) and (alphas[j] >0)):
				j = selectJrand(i,m)
				fXj = float(multiply(alphas,labelMat).T*(dataMatrix*dataMatrix[j,:].T)) + b
				Ej = fXj - float(labelMat[j])
				alphaIold = alphas[i].copy()
				alphaJold = alphas[j].copy()
				if(labelMat[i] != labelMat[j]):
					L = max(0, alphas[j] - alphas[i])
					H = min(c, c + alphas[j] - alphas[i])
				else:
					L = max(0, alphas[j] + alphas[i] + c)
					H = min(c, alphas[j] + alphas[i])
				if L==H:print "L==H";continue
				eta = 2.0 * dataMatrix[i,:]*dataMatrix[j,:].T - dataMatrix[i,:]*dataMatrix[i,:].T - dataMatrix[j,:]*dataMatrix[j,:].T
				if eta >= 0:print "eta>=0";continue
				alphas[j] -= labelMat[j]*(Ei-Ej)/eta
				alphas[j] = clipAlpha(alphas[j],H,L)
				if(abs(alphas[j] - alphaJold) < 0.00001):
					print "j not moving enough";continue
				alphas[i] += labelMat[j]*labelMat[i]*(alphaJold - alphas[j])
				b1 = b - Ei - labelMat[i]*(alphas[i]-alphaIold)*dataMatrix[i,:]*dataMatrix[i,:].T - labelMat[j]*(alphas[j]-alphaJold)*dataMatrix[i,:]*dataMatrix[j,:].T
				b2 = b - Ej - labelMat[i]*(alphas[i]-alphaIold)*dataMatrix[i,:]*dataMatrix[j,:].T - labelMat[j]*(alphas[j]-alphaJold)*dataMatrix[j,:]*dataMatrix[j,:].T
				if(0 < alphas[i]) and (c > alphas[i]): b = b1
				elif(0 < alphas[j]) and (c > alphas[j]): b = b2
				else: b = (b1 + b2)/2.0
				alphaPairsChanged += 1
				#print "alphasI: %f alphasJ: %f b:%f " % (alphas[i],alphas[j],b)
				#print "iter: %d i: %d, pairs changed %d" % (iter,i,alphaPairsChanged)
		if(alphaPairsChanged == 0):iter += 1
		else: iter = 0
		#print "iteration number: %d" % iter
	return b,alphas
				
def calcW(alphas,dataMat,labelMat):
	dataMatrix=mat(dataMat)
	labelMatrix=mat(labelMat).transpose()
	m,n = shape(dataMatrix)
	w = zeros((n,1))
	for i in range(m):
		w += multiply(alphas[i]*labelMatrix[i],dataMatrix[i,:].T)
	return w

#画图
def plotBestFit(weights,b,dataMat,labelMat):
	import matplotlib.pyplot as plt
	#dataMat,labelMat=loadData(plot)
	dataArr = array(dataMat)
	n = shape(dataArr)[0]
	xcord1 = [];ycord1 = []
	xcord2 = [];ycord2 = []
	for i in range(n):
		if int(labelMat[i]) == 1:
			xcord1.append(dataArr[i,0]);ycord1.append(dataArr[i,1])
		else:
			xcord2.append(dataArr[i,0]);ycord2.append(dataArr[i,1])
	fig = plt.figure()
	ax = fig.add_subplot(111)
	ax.scatter(xcord1, ycord1, s=30, c='red',marker='s')
	ax.scatter(xcord2, ycord2, s=30, c='green')
	x = arange(-3.0, 3.0, 0.1)
	y = (1 - b[0,0] - weights[0]*x)/weights[1]
	ax.plot(x,y)
	plt.xlabel('X1');plt.ylabel('X2')
	plt.savefig('/data/rd/www/lvyunhe/plot_svm', format='png')


if len(sys.argv) < 3:
	print "python svm.py <xlabel> <ylabel>"
	sys.exit(0)
train_data='/data/rd/lvyunhe/machine_leaning/test_svm.dat'
dataArr,labelMat=loadData(train_data)
b,alphas = smoSimple(dataArr,labelMat,0.6,0.001,40)
w = calcW(alphas,dataArr,labelMat)
#weights=gradAscent(array(dataArr),labelMat)
#print "b: ",b,"\nalphas:\n",alphas,"\nw:\n",w
plot='/data/rd/www/lvyunhe/plot_svm'
plotBestFit(w,b,dataArr,labelMat)
x=sys.argv[1]
y=sys.argv[2]
res =  float(x)*w[0,0] + float(y)*w[1,0] + b[0,0]
if res > 1:
	print "类别为：1"
elif res < -1:
	print "类别为：-1"
else: print "error"

