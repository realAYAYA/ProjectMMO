# 开发笔记

## Animation Blueprint(Lyra)

### 1 基础移动
1. 创建State Machine后，留意使用Inertialization节点  
2. Thread Safe Function 只有线程安全函数才可以  
3. 在标记Thread Safe的函数中，建议通过使用Property节点来捕获希望用于计算的变量  
4. 通过设置Pure函数（并将返回值名称设为“ReturnValue”），可以生成自定义可以被捕获的Property  
5. 在Locomotion中的条件节点中，左侧面板Transition-Automatic Rule Based Transition打勾，此处设置的作用是在条件之前的动画播放完成后，条件允许下会自动播放条件之后的动画（例如，奔跑启动动作---奔跑循环动作）  
6. 条件节点左侧面板中的Transition Rule Sharing，可以通过提升为共享（自己命名），作用相当于共享其判断逻辑的代码，这样在设置其它条件节点时，如果逻辑相同，可以通过这直接套用，节省编写工作  
7. 条件节点左侧面板中的Blend Settings---Blend Profile，左右是在动画过渡时可以提供较好的动作混合（从站立到起跑）  
8. 创建Animation Layer Interface蓝图，可以在其中声明各种姿势的动画接口，提高动画蓝图的复用程度  

9. 搞清楚ABP_MCharacterBase ALI_MCharacter ABP_Item 以及 BP_PlayerCharacter 几个蓝图之间的关系  
10. 搞清楚Sequence Player和Sequence Evaluator的区别，后者适合根据Explicit Time值来决定动画播放程度  
11. Sequence Evaluator节点，右侧面板，Explicit Time改为动态值，目的是在程序中可以动态调整播放程度；  
12. Sequence Evaluator节点，右侧面板，
## GAS

1. git创建分支
2. 创建或修改文件
3. 合并之后提交
4. 冲突解决

# 致敬 玩梗 笔记
1. 近战对我们有利！  
2. 西玛阿姨这个人物的致敬（你不要跟我讲这个人扭曲了很可惜，你要问自己换成你有没有得选）  
3. A：西玛大人请撤退吧！  B:你让我退到哪里去啊！  
4. 一种表面嵌了许多铁球的棍棒双手武器，叫“前夫”