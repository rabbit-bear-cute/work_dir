<?php

define("REDIS_HOST", "127.0.0.1");
define("REDIS_PORT", 6379);
define("REDIS_DB", 2);
//
$redis = new Redis();
$redis->connect(REDIS_HOST, REDIS_PORT);
$redis->select(REDIS_DB);
/////////////////////////
$fp = fopen("part-00000", "r");
//$fp = fopen("part-1", "r");
while($line = fgets($fp)) {
	if(($line = trim($line)) == "") {
		continue;
	}
	//
	list($id, $word_seg, $cid, $title) = explode("\t", $line);
	$info = $cid . "\t" . $title;
	$redis->hSet("MM:TITLESEG", $id, $info);//存入hash表key(MM:TITLESTE),链表的key为id,value为info
	$word_seg = explode(" ", $title);
	foreach($word_seg as $word) {
		$redis->sAdd($word, $id);//以word为key，id为值存入集合
	//echo $word . "\t" . $id ."\n";
	}
}
//
fclose($fp);

