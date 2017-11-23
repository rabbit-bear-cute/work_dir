<?php
$python_file = '/home/rank/lvyunhe/ElasticSearch/script/display_page_search.py';
if(isset($_GET['search_tag']) && trim($_GET['search_tag']) != "") {
    $search_tag = $_GET['search_tag'];
}
if(isset($_GET['query']) && trim($_GET['query']) != "") {
    $query = $_GET['query'];
}
if(isset($_GET['topn']) && trim($_GET['topn']) != "") {
    $topn = $_GET['topn'];
}
$output = shell_exec('python '. $python_file . ' ' . $query . ' '. $topn . ' ' . $search_tag);
#$output = shell_exec('python '. $python_file . ' ' . 'title' . ' ' . '食品饮料' . ' ' . 'match');
#echo $output;
$lines = explode("\n",$output);
$output = array();
for($i=0;$i<count($lines)-1;$i++){
    $terms = explode("\t",$lines[$i]);
    if(count($terms) != 5){
        continue;
    }
    $output1 = array('score'=>$terms[2],'title'=>$terms[4]);
    $output2 = array('number'=>$i+1,'url'=>$terms[3],'total'=>$terms[0],'max_score'=>$terms[1],'details'=>$output1);
    $output[]=$output2;
}
echo str_replace("\\","",json_encode($output,JSON_UNESCAPED_UNICODE))."<br>";
?>

