/*
 * ReadMe.txt
 * author: Xian Wu
 * mail: wuxian94@pku.edu.cn
 */

数据说明：
数据来源于ACL Anthology，经过预处理[1]和初步分析[2]后产生。
数据集包含了2503篇workshop文章，划分为10个类别。附有标题、作者、摘要、引用信息和正文向量。

数据集涉及的id列表保存在id_list.txt中。

以下文件的格式均为，每行分为两部分，以空格分隔。第一部分为文章id，第二部分为具体内容，为了保证正确性，以json字符串格式保存。
如果需要保存的是一个列表（数组），那么以"|"符号作为分隔符，保存为字符串。例如：W00-1403 "Daniel Marcu|Lynn Carlson|Maki Watanabe"

具体内容如下：
    abstracts.txt: 一个字符串，文章摘要。
    authors.txt: 一个列表，作者列表。
    docvecs.txt: 一个列表，正文向量，保存了向量的各个维度的值。
    known_citings.txt: 链接预测作业使用的数据，一个列表，文章引用的对象id。
    titles: 一个字符串，文章标题。
以上文件涉及的id都仅限于id_list.txt出现过的id中，且保证每个文件都为2503行。（换行符为\r，在windows下请用程序读取或除了记事本之外的文本编辑器打开）

对于社区划分作业，有已分类的信息，并划分好train与test集合：train_list.txt与test_list.txt。其内容每行为文章id与label编号。label的真实名称见label_name.txt。

值得注意的是，受到文本化技术的限制，部分文章存在些问题，约300篇文章因文本化问题，丢失了摘要和正文信息。因此，这部分文章的摘要为<NoAbstract>，正文向量是利用标题，由title2doc模型[3]推断得到。

[1]. 利用了ocr++软件包。
[2]. 阮翀师兄于2014年筛选得到。
[3]. 吴先的毕设论文。