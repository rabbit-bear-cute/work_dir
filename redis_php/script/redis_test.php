<?php
//连接本地的 Redis 服务
$redis = new Redis();
$redis->connect('127.0.0.1', 6379);

$redis->Lrem("tutorial-list", 0, 1);
$redis->Lrem("tutorial-list", 0, 2);
$redis->Lrem("tutorial-list", 0, 3);
//存储数据到列表中
//$redis->lpush("tutorial-list", "Redis");
//$redis->lpush("tutorial-list", "Mongodb");
//$redis->lpush("tutorial-list", "Mysql");
// 获取存储的数据并输出
$arList = $redis->lrange("tutorial-list", 0 ,-1);
//echo "Stored string in redis";
print_r($arList);
//$password = '123456';
 
//$redis->auth($password);
$redis->Lrem("mylist", 0, "h");
$redis->Lrem("mylist", 0, "e");
$redis->Lrem("mylist", 0, "l");
$redis->Lrem("mylist", 0, "w");
$redis->Lrem("mylist", 0, "o");
$redis->Lrem("mylist", 0, "r");
$redis->Lrem("mylist", 0, "d");
 
$arr = array('h','e','l','l','o','w','o','r','l','d');
//$num = count($arr);
 
foreach($arr as $k=>$v){
	 
//	$redis->rpush("mylist",$v);
}
$armylist = $redis->lrange("mylist",0,-1);
print_r($armylist);

$value = $redis->lpop('mylist');
 
if($value){
	 
	echo "出队的值".$value."\n";
	 
}else{
	 
	echo "出队完成"."\n";
	 
}
?>
