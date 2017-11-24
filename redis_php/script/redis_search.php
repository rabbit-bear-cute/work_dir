<?php

define("REDIS_HOST", "127.0.0.1");
define("REDIS_PORT", 6379);
define("REDIS_DB", 2);
//
if(isset($_GET['query']) && trim($_GET['query']) != "") {
	$query = $_GET['query'];
}
//
function segment($type) {
	$url = "http://idc01-nlp-web-vip00:8400/segmenter?type=basic&query=" . urlencode($type);
	$query = file_get_contents($url);
	$query_seg = explode("AND", $query);
	//
	return $query_seg;
}
//
$redis = new Redis();
$redis->connect(REDIS_HOST, REDIS_PORT);
$redis->select(REDIS_DB);
//
//$fp = fopen("php://stdin", "r");
//while($query = fgets($fp)) {
    $count = array();
    $result = array();
    $detail = array();
	$query_seg = segment($query);
	$words = trim($query_seg[0]);
	foreach($query_seg as $word) {
		$count[$word] = $redis->scard(trim($word));#获取集合key为word的成员数，key->word,value->id
		$redis->SinterStore($words, trim($word), $words);#取集合word和words的交集存入words,交集即集合word和words中同时包含的id
		//echo $word . "\t" .$count . "\n";
	}
	$ids = $redis->sMembers($words);#返回集合中的所有成员，即所有query_seg同时出现的id
	foreach($ids as $id) {
		$info = $redis->hGet("MM:TITLESEG" , $id);#取回哈希表MM:TITLESTE中指定字段id的值info(cid,title)
		list($cid, $title) = explode("\t", $info);
		$result[] = array('taobao_id' => $id, 'cid' => $cid, 'title' => $title);
		//echo $id . "\t" . $cid . "\t" . $title ."<br>";
	}
	foreach($count as $word => $word_count) {
		$detail[] = array('word' => $word, 'word_count' => $word_count);
		//echo $word . "\t" . $word_count . "<br>";
	}
	$output = array('query' => $query_seg , 'detail' => $detail, 'result' => $result);
	echo json_encode($output);
//}
/////////////////////////
?>

