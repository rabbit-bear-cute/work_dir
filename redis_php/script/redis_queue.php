<?php
 
define("REDIS_HOST", "127.0.0.1");
define("REDIS_PORT", 6379);
define("REDIS_DB", 3);
//
if(isset($_GET['name']) && trim($_GET['name']) != "") {
	$name = $_GET['name'];
}

if(isset($_GET['mode']) && trim($_GET['mode']) != "") {
	$mode = $_GET['mode'];
}

if(isset($_GET['data']) && trim($_GET['data']) != "") {
	$data = $_GET['data'];
}
//
$redis = new Redis();
$redis->connect(REDIS_HOST, REDIS_PORT);
$redis->select(REDIS_DB);
//
if($mode == "push") {
	$redis->lpush($name,$data);
	$res['result'] = "push";
}
else if($mode == "pop") {
	$value = $redis->lpop($name);
	if($value){
		$res['result'] = $value;
	}else{
		$res['reslut'] = "NULL";
	}
}else{
	$res['reslut'] = "please input mode=pop or mode=push ";
}
echo json_encode ($res);
