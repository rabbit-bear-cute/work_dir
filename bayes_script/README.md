### leaf_dict
* cid_leaf_all.dict: 类目体系总的cid
* stop_leaf.dict: 一级类目为“书籍/杂志/报纸”、“其他”及“自用闲置转让”的cid
* cid_leaf.dict: 出去stop_leaf.dict后的所有类目

### word_seg
* 以cid_leaf.dict为词典进行训练数据的筛选并切词

### cid_term
* 前三列为key 以前三列排序 以原训练集作为输入
* 输出:
* cid	1	term	term_num	
* cid	0	*	cid_num	

### cid
* 用来计算cid.dict，以cid_term输出作为输入
* 输出:
* cid	cid_count	cid_total_count	

### term_bayes
* 首列为key，前两列排序。整合数据，分别以cid、cid_term的输出作为输入
* 输出:
* cid	term	term_num	cid_count	cid_total_count	

### bayes_calc
* 用于贝叶斯计算同
* 输出为最终的bayes词典相关数据



