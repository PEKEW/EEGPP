EEGPP (EEG + Git) 系统设计文档

命令格式：

`{}` 表示操作类型 `{any}` 表示任意类似的操作 `{data}` 表示必须要有data关键字的操作
`eegpp` 操作头
`()` 表示不可缺少的参数
`[]` 表示可以缺少的参数

1. 系统目标
设计并实现一个简单的具有类似git版本控制的eeglab

2. 核心功能
初始化仓库：创建一个新的仓库，用于跟踪文件更改。 需要参数（*.eegpp / *.set）
`eegpp init (filename)`
添加文件：将文件更改添加到暂存区。
`{any} eegpp (op) [param]`
存储更改：将暂存区的更改永久记录到仓库历史中
`eegpp save (info)`
查看历史：查看提交历史和具体更改
`eegpp his` 输出版本控制的历史操作
回滚更改：能够回到历史中的某个版本。
`eegpp roll (ref)`
`ref: xx/xx/xx`
EEGLAB的主要功能： 重参考 绘图 中心点重设 滤波 FFT 等
`{data} eegpp data (op) [param]`
分支操作
reset
find
checkout 等
`eegpp reset (parent | child) [xx/xx/xx]`
`eegpp find (parent | child)`
`eegpp checkout (xx/xx/xx | idx)`
3. 组件设计
3.1 数据结构
- EEGRepo: 代表一个版本库，包含以下属性：
    - .eegv/：隐藏文件夹，用于存储所有版本控制信息 (具体这里存储什么？)
    - eegobjs/：存储所有的提交对象和数据 每个对象都有一个唯一的SHA-1哈希作为其名称，通常存储为文件内容的压缩版本
      - 包含：
        父提交的引用
        提交信息
        EEG DATA
    - refs/：包含指向提交对象的引用，如分支和标签 
    - HEAD：指向当前分支的指针 一个文件，记录当前检出的提交或分支，是一个到refs中某个引用的链接
    - Commit Object：每个提交都创建一个提交对象，包含：
        父提交的引用
        提交的快照（即该次提交时的文件状态）
        提交信息



结构示例（对象用文本文件存储）

parent: 1234abcd
tree: abcd1234
time: 20000000
message: Initial commit of my project
