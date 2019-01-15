<?php

/**
 * Basic implementation for accessing the domain access data.
 */
class DomainAccess implements DomainAccessInterface {

  /**
   * {@inheritdoc}
   */
  public static function checkRuleExists($nid, $gid, $realm) {
    $data = static::getByNid($nid);
    $exists = array_filter($data, function($item) use ($gid, $realm) {
      return $item['gid'] == $gid && $item['realm'] == $realm;
    });
    return !empty($exists);
  }

  /**
   * {@inheritdoc}
   */
  public static function getGidByNidAndRealm($nid, $realm) {
    $data = static::getByNid($nid);
    $gid = FALSE;
    if (!empty($data)) {
      $gid = array_reduce($data, function($carry, $item) use ($realm) {
        if ($item['realm'] == $realm) {
          return $item['gid'];
        }
        return $carry;
      }, FALSE);
    }
    return $gid;
  }

  /**
   * {@inheritdoc}
   */
  public static function getDataForNids($nids) {
    $results = array();
    foreach ($nids as $nid) {
      $data = static::getByNid($nid);
      if (!empty($data)) {
        $results = array_merge($results, array_map(function($item) use ($nid) {
          return (object) array(
            'nid' => $nid,
            'gid' => $item['gid'],
            'realm' => $item['realm'],
          );
        }, $data));
      }
    }
    return $results;
  }

  /**
   * {@inheritdoc}
   */
  public static function reassignDomain($old_domain_id, $new_domain_id) {
    $nids = db_query("SELECT nid FROM {domain_access} WHERE realm = 'domain_id' AND gid = :domain_id", array(':domain_id' => $new_domain_id))->fetchAllAssoc('nid');
    $query = db_update('domain_access')
      ->condition('gid', $old_domain_id)
      ->condition('realm', 'domain_id')
      ->fields(array('gid' => $new_domain_id));
    // We cannot update using a subquery, so be sure to exclude duplicates.
    if (!empty($nids)) {
      $query->condition('nid', array_keys($nids), 'NOT IN');
    }
    $query->execute();
  }

  /**
   * {@inheritdoc}
   */
  public static function deleteDomain($domain_id) {
    db_delete('domain_access')
      ->condition('gid', $domain_id)
      ->condition('realm', 'domain_id')
      ->execute();
  }

  /**
   * {@inheritdoc}
   */
  public static function deleteNode($nid) {
    db_delete('domain_access')
      ->condition('nid', $nid)
      ->execute();
  }

  /**
   * {@inheritdoc}
   */
  public static function insertValues($values) {
    $query = db_insert('domain_access')->fields(array('nid', 'gid', 'realm'));
    foreach ($values as $record) {
      $query->values($record);
    }
    $query->execute();
  }

  /**
   * Loads the domain access data for a node id.
   *
   * @param int $nid
   *  The node id.
   * @return array
   */
  protected static function getByNid($nid) {
    $results = db_query('SELECT da.nid, da.gid, da.realm from {domain_access} da WHERE da.nid = :nid', array(':nid' => $nid), array('fetch' => PDO::FETCH_ASSOC));
    $data = array();
    foreach ($results as $result) {
      $data[] = array(
        'gid' => $result['gid'],
        'realm' => $result['realm'],
      );
    }
    return $data;
  }
}
