# SurfaceQualityInspection

## 项目介绍
这个项目起初只是做一些尝试，作为一个CAD，CG方向研究生的trainning，来熟悉一些三维图形处理
	的基本方法和算法，后来机缘巧合全球某知名汽车厂商，有关于一些曲面质量检测和分类的需求，才开
	始正式立为项目。经过一年的研究和开发，最后还是获得了很好的结果。项目不仅仅是针对汽车生产大
	有帮助，对于工业生产的很多领域同样适用。下面就来简单介绍一下这个项目，关于算法的原理和实现
	细节，项目中有文档。
	
在汽车制造行业中,车体外表面的外观是衡量一个车身质量和美观的重要标 准。工业界对汽车外表质量
检测要求越来越高,但是目前并没有很好的方法对其 进行自动化检测和分类,大多方法需要复杂的用户
操作。自动化检测和分类方式 不仅有利于提高检测精度,而且有利于降低生产成本。因此,自动化曲面
缺陷检 测和分类方法是非常必要的。
	分为曲面缺陷自动检测和缺陷分类两个模块。在曲面缺陷自动检测模块 中首先实现了 ReflectionMap算法,该方法的检测结果依赖于视点的位置,仍然 需要用户的交互操作。为了使检测结果不依赖于视点位置,本文提出将 Reflection Map 算法在有效视点区域计算积分。最后对于无法整体对齐的情况, 提出并实现了局部对齐后视点积分 Reflection Map 算法,该算法还可以有效的 检测出不同尺度的缺陷。并且该算法可以实现全自动化缺陷检测。
	 曲面缺陷自动检测模块中的算法都是基于标准模型和制造模型之间的对比算法。对于不存在标准模型的情况,曲面缺陷自动检测算法将无法满足用户需求。 因此,本文提出并实现曲面缺陷分类算法来对制造模型上的缺陷进行分类。首先, 本文应用了曲面局部高度图的泽尼克系数,曲率泽尼克系数,曲面光顺后的曲率 泽尼克系数,曲率统计直方图作为曲面描述符。分别用这些描述符作为训练数据的特征向量。本文应用支持向量机算法来训练得到一个类型分类器。应用分类器 自动对曲面上的缺陷进行分类标注。最后,本文根据实验结果对这几种曲面描述 符的优缺点进行分析和讨论。
	
## Introduction

In the automobile manufacturing industry, the appearance of the body outer surface is a measure of body mass and an important aesthetic criteria. The requirement of the surface quality detection has become increasingly high. However, there is no good way to its automatic detection and classification. And most applications require complex user interaction. Automatic detection and classification not only help to improve the accuracy of detection, but also reduce production costs. Therefore, automatic surface defect detection and classification methods seem indispensable.
   
This paper include automatic surface defect detection module and surface defects classification module. In automatic surface defect inspection module, firstly, I implement Reflection Map algorithm. The result of this algorithm depend on the position of viewpoint and need lots of user interaction. To solve this problem, this paper presents the Integral Reflection Map algorithm which integrate the defect value in valid viewpoint area. Finally, this paper propose Local Alignment Integral Reflection Map algorithm to automatically deal with some special cases which two models can’t be aligned globally. This algorithm can also detect different scale defects.

The algorithms in surface defects detection module are based on reference model and manufacture model. For the absence of the reference model, surface defects detection algorithms will not be able to meet customer requirements. Therefore, I propose and implement the surface defects classification algorithm to classify defects on manufacture model. Firstly, I apply local height map Zernike coefficient, curvature Zernike coefficient, smoothing curvature Zernike coefficient and curvature histogram as surface descriptor. Secondly, this paper use Support Vector Machine to obtain a classifier. I label defects classification on manufacture model by using this classifier. Finally, this paper analyze and discuss the advantages and disadvantages of these different kinds of surfaces descriptor.
