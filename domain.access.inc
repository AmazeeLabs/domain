<?php

class DomainAccess {

  public static function updateCache($nid) {
    $results = db_query('SELECT da.nid, da.gid, da.realm from {domain_access} da WHERE da.nid = :nid', array(':nid' => $nid), array('fetch' => PDO::FETCH_ASSOC));
    $data = array();
    foreach ($results as $result) {
      $data[] = array(
        'gid' => $result['gid'],
        'realm' => $result['realm'],
      );
    }
    cache_set($nid, $data, 'cache_domain_access');
  }

  public static function getByNid($nid, $is_recursive = FALSE) {
    if ($cache = cache_get($nid, 'cache_domain_access')) {
      return $cache->data;
    }
    if ($is_recursive) {
      return array();
    }
    self::updateCache($nid);
    return self::getByNid($nid, TRUE);
  }

  public static function checkRuleExists($nid, $gid, $realm) {
    // Used for queries:
    // - db_query("SELECT COUNT(nid) FROM {domain_access} WHERE nid = :nid AND gid = 0 AND realm = :realm", array(':nid' => $nid, ':realm' => 'domain_site'))->fetchField()

    $data = self::getByNid($nid);
    $exists = array_filter($data, function($item) use ($gid, $realm){
      return $item['gid'] == $gid && $item['realm'] == $realm;
    });
    return !empty($exists);
  }

  public static function getGidByNidAndRealm($nid, $realm) {
    // Used for queries like:
    // - $id = db_query("SELECT gid FROM {domain_access} WHERE nid = :nid AND realm = :realm ORDER BY gid", array(':nid' => $nid, ':realm' => 'domain_id'))->fetchField();
    $data = self::getByNid($nid);
    $gid = FALSE;
    if (!empty($data)) {
      foreach ($data as $item) {
        if ($item['realm'] == $realm) {
          return $item['gid'];
        }
      }
    }
    return $gid;
  }

  public static function getDataForNids($nids) {
    // Used for queries like:
    // - db_query("SELECT nid, gid, realm FROM {domain_access} WHERE nid IN (:nid)", array(':nid' => array_keys($node_ids)));
    $results = array();
    foreach ($nids as $nid) {
      $data = self::getByNid($nid);
      if (!empty($data)) {
        foreach ($data as $value) {
          $results[] = (object)array(
            'nid' => $nid,
            'gid' => $value['gid'],
            'realm' => $value['realm'],
          );
        }
      }
    }
    return $results;
  }
}
