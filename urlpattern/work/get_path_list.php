<?php
function get_path($url) {
    $query_url = "http://10.2.11.64:8080/lvyunhe/url_path_match.php?url=";
    $ask_url = $query_url . urlencode($url);
    $result = file_get_contents($ask_url);
    $res_list = array();
    $res_list = json_decode($result, TRUE);
    return $res_list;
}
 
$fp = fopen("php://stdin", "r");
while($url = fgets($fp)) {
    if(($url = trim($url)) == "") {
        continue;
    }
    $match_res = get_path($url);
    if(!empty($match_res['pattern'])) {
        echo $match_res['url'] . "\t" . $match_res['pattern'] . "\n";
    }
}
?>
