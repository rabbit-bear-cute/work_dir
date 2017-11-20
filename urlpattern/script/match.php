<?php
include_once(dirname(__FILE__) . "/match_core.php");

function work() {
    $fp = fopen("php://stdin", "r");
    while($url = fgets($fp)) {
        if(($url = trim($url)) == ""){
            continue;
        }
        $match_res = path_match($url);
        if(empty($match_res['pattern'])) {
            $content =  date("Y-m-d H:i:s") . "\t" . $match_res['url'] . "\n";
            $log_name = "./log/log." . date("Y-m-d");
            error_log($content, 3, $log_name);
        }else{
            echo $match_res['url'] . "\t" . $match_res['pattern'] . "\n";
        }
    }
}
/*if(is_file('url.log')) {
    unlink("url.log");
}*/
work();
?>
