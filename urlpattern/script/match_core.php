<?php
define("REDIS_HOST", "127.0.0.1");
define("REDIS_PORT", "6379");
define("REDIS_DB", 7);

//pattern词典加载
function path_dict_load() {
    $redis = new Redis();
    $redis->connect(REDIS_HOST, REDIS_PORT);
    $redis->select(REDIS_DB);
    $redis->del("path_set");
    $fp = fopen("/home/rank/lvyunhe/zz_task/urlpath/php_serve/pattern_data", "r");
    while($path = fgets($fp)) { 
        if(($path = trim($path)) == ""){
            continue;
        }
        $redis->sAdd("path_set", $path);
    }
    return $redis;
}
//pattern匹配
function path_match($url) {
    //url解析
    $del_host = strstr($url, ":");//返回http/https之后的内容
    $domain_path = substr($del_host, 3);//返回://之后的内容
    $fp_path = strstr($domain_path, "/");//返回文件路径path
    //pattern读取
    $redis = path_dict_load();
    $path_list = null;
    $path_arr = $redis->smembers("path_set");
    //处理图片url
    $img_tag = strrchr($fp_path, ".");
    $img_flag = preg_match('/jpg|png|gif/i', $img_tag);
    if($img_flag) {
        $path_list = "img_path";
        $match_res = array('url' => $url, 'pattern' => $path_list);
        return $match_res;
    }
    //处理数字url
    $digital_tag_tmp = strstr($fp_path, ".", true);//返回.html、.stml、.json等前面的部分
    if(!empty($digital_tag_tmp)) {
        $digital_tag = strrchr($digital_tag_tmp, "/");
    }else{
        $digital_tag = strrchr($fp_path, "/");
    }
    $digital_flag = preg_match('/\/\d+.*\d+$|\/\d/', $digital_tag);
    //echo $matches[0] . "\t" . $matches[1] ."\t".$mathes[2];
    if($digital_flag) {
        if(isset($path_list)) {
            $path_list = $path_list . ",digital_path";
        }else{
            $path_list = "digital_path";
        }
    }
    foreach($path_arr as $path) {  
        $flag = preg_match("'$path'", $fp_path);
        if($flag) {
            if(isset($path_list)) {
                $path_list = $path_list . "," . $path;
            }else{
                $path_list = $path;
            }
        }
    }
    $match_res = array('url' => $url, 'pattern' => $path_list);
    return $match_res;
}
//$result = path_match("http://www.babytree.com/learn/article/3289");
//echo $result['url'] . "\n" . $result['pattern'] . "\n";
?>
