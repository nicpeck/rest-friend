<?php
namespace RestFriend;

class API {
  public $endpoints = [];

  // private $config = array(
  //   "basePath" => "",
  //   "returnType" => "application/json"
  // );
  //
  // public function __construct($config = array()){
  //   foreach($config as $key => $value){
  //     $this->config[$key] = $value;
  //   }
  // }
  //
  // public function getConfig($key=NULL){
  //   if(gettype($key) != 'string'){
  //     return $this->config;
  //   }else if(isset($this->config[$key])){
  //     return $this->config[$key];
  //   }else{
  //     return NULL;
  //   }
  // }
  // public function setConfig($key,$value){
  //   $this->config[$key] = $value;
  // }

  public function addEndpoint($method,$path,$callback,$access=array()){
    $path = trim($path,"/");
    if(!isset($this->endpoints[$path])){
      $this->endpoints[$path] = array();
    }
    $this->endpoints[$path][$method] = new \RestFriend\Endpoint($callback,$access);
  }

  public function endpointCallback($method,$path){
    if(!isset($this->endpoints[$path])){
      $this->endpoints[$path] = array();
    }
  }

  public function getMenu(){
    $items = array();
    foreach($this->endpoints as $path => $methods){
      $items[$path] = array(
      	'title' => 'REST FRIEND API OUTPUT',
      	'delivery callback' => 'restfriend_core_endpoint_delivery',
      	'access callback' => true,
      	'type' => MENU_CALLBACK
      );
    }
    return $items;
  }

}

class Endpoint {
  public $callback = NULL;
  public function __construct($callback,$access){
    $this->callback = $callback;
    $this->access = $access;
  }
}

class Token {
  public $value = NULL;
  // public $expires = NULL;
  private $expiry = 0;
  public function __construct($uid){
    $uid = intval($uid);
    if($uid==0){
      return false;
    }

    db_delete('restfriend_core_authtokens')
    ->condition('uid',$uid)
    ->execute();

    $count = 1;
    for($i = 0; ($count > 0) && $i < 100; $i++){
      $this->value = $this->generateToken();
      $count = db_select('restfriend_core_authtokens')->fields('restfriend_core_authtokens')->condition('token',$this->value)->execute()->rowCount();
    }

    $this->expires = $this->expiry>0?time()+$this->expiry:NULL;


    $result = db_insert('restfriend_core_authtokens')
    ->fields(array(
      "uid" => $uid,
      "token" => $this->value,
      "expires" => $this->expires
    ))
    ->execute();

  }
  private function generateToken($uid){
    return strtoupper(base_convert(md5($uid.time().rand()),16,36));
  }
}
