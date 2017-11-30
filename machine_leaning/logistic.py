#!/usr/bin/env python
# coding=utf-8
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
        dataMat.append([1.0, float(lineArr[0]), float(lineArr[1])])
        labelMat.append(int(lineArr[2]))
    return dataMat,labelMat

#sigmoid函数
def sigmoid(inX):
    return 1.0/(1+exp(-inX))

#梯度上升法
def gradAscent(dataMatIn, classLabels):
    dataMatrix = mat(dataMatIn)
    labelMat = mat(classLabels).transpose()
    m,n = shape(dataMatrix)
    alpha = 0.001
    maxCycles = 500
    weights = ones((n, 1))
    for k in range(maxCycles):
        h = sigmoid(dataMatrix*weights)
        error = (labelMat - h)
        weights = weights + alpha * dataMatrix.transpose()*error
    return weights

#随机梯度上升法
def stocGradAscent0(dataMatrix, classLabels):
    m,n = shape(dataMatrix)
    alpha = 0.01
    weights = ones(n)
    for i in range(m):
        h = sigmoid(sum(dataMatrix[i]*weights))
        error = classLabels[i] - h
        weights = weights + alpha * error*dataMatrix[i]
    return weights

#改进的随机梯度上升法
def stocGradAscent1(dataMatrix, classLabels, numIter=150):
    m,n = shape(dataMatrix) #返回矩阵行和列
    weights = ones(n)
    for j in range(numIter):
        dataIndex = range(m)
        for i in range(m):
            alpha = 1/(1.0 +j+i)+0.01
            randIndex = int(random.uniform(0,len(dataIndex))) #随机生成实数
            h = sigmoid(sum(dataMatrix[randIndex]*weights))
            error = classLabels[randIndex] - h
            weights = weights + alpha * error * dataMatrix[randIndex]
            del(dataIndex[randIndex])
    return weights

#计算类别
def classifyVector(inX, weights):
    prob = sigmoid(sum(inX*weights))
    if prob > 0.5: return 1.0
    else: return 0.0

#画图
def plotBestFit(weights,plot,dataMat,labelMat):
	import matplotlib.pyplot as plt
	#dataMat,labelMat=loadData(plot)
	dataArr = array(dataMat)
	n = shape(dataArr)[0]
	xcord1 = [];ycord1 = []
	xcord2 = [];ycord2 = []
	for i in range(n):
		if int(labelMat[i]) == 1:
			xcord1.append(dataArr[i,1]);ycord1.append(dataArr[i,2])
		else:
			xcord2.append(dataArr[i,1]);ycord2.append(dataArr[i,2])
	fig = plt.figure()
	ax = fig.add_subplot(111)
	ax.scatter(xcord1, ycord1, s=30, c='red',marker='s')
	ax.scatter(xcord2, ycord2, s=30, c='green')
	x = arange(-3.0, 3.0, 0.1)
	y = (-weights[0]-weights[1]*x)/weights[2]
	ax.plot(x,y)
	plt.xlabel('X1');plt.ylabel('X2')
	plt.savefig(plot, format='png')

#读取实验数据
def colicTest():
    fpTrain = open('test.dat')
    fpTest = open('test.dat')
    trainSet = []; trainLabels = []
    for line in fpTrain.readlines():
        currLine = line.strip().split()
        lineArr = []
        for i in range(2):
            lineArr.append(float(currLine[i]))
        trainSet.append(lineArr)
        trainLabels.append(float(currLine[2]))
	trainWeights = stocGradAscent1(array(trainSet), trainLabels,500)
	file_name='/data/rd/www/lvyunhe/plot_horse'
	#plotBestFit(trainWeights,file_name,trainSet,trainLabels)
    errorCount = 0; numTestVec = 0.0
    for line in fpTest.readlines():
        numTestVec += 1.0
        currLine = line.strip().split()
        lineArr = []
        for i in range(2):
            lineArr.append(float(currLine[i]))
        #print classifyVector(array(lineArr), trainWeights)
        if int(classifyVector(array(lineArr), trainWeights)) != int(currLine[2]):
            errorCount += 1;
    errorRate = (float(errorCount)/numTestVec)
    print "the error rate of this test is: %f" % errorRate
    return errorRate


def multiTest():
    numTests = 10; errorSum=0.0
    for k in range(numTests):
        errorSum += colicTest()
    print "after %d iterations the average error rate is:%f" % (numTests, errorSum/float(numTests))

if len(sys.argv) < 3:
	print "python logistic.py <ylabel> <zlabel>"
	sys.exit(0)
train_data='/data/rd/lvyunhe/machine_leaning/test_logistic.dat'
dataArr,labelMat=loadData(train_data)
weights=stocGradAscent1(array(dataArr),labelMat)
#weights=gradAscent(array(dataArr),labelMat)
print weights
plot='/data/rd/www/lvyunhe/plot_logistic'
plotBestFit(weights,plot,dataArr,labelMat)
y=sys.argv[1]
z=sys.argv[2]
label = classifyVector(array([1.0,float(y),float(z)]),weights)
print label

