<?php

/**
 * Interface for accessing the domain access data.
 */
interface DomainAccessInterface {

  /**
   * Checks if a specific rule exists.
   *
   * @param int $nid
   *  The node id.
   * @param int $gid
   *  The gid.
   * @param string $realm
   *  The realm.
   * @return bool
   */
  public static function checkRuleExists($nid, $gid, $realm);

  /**
   * Returns a gid, given the node id and the realm.
   *
   * @param int $nid
   *  The node id.
   * @param string $realm
   *  The realm.
   * @return int | FALSE
   */
  public static function getGidByNidAndRealm($nid, $realm);

  /**
   * Returns the access data for a set of node ids.
   *
   * @param array $nids
   *  An array of node ids.
   * @return array
   */
  public static function getDataForNids($nids);

  /**
   * Reassigns domain access data to a new domain.
   *
   * @param int $old_domain_id
   *  The old domain id.
   * @param $new_domain_id
   *  The new domain id.
   */
  public static function reassignDomain($old_domain_id, $new_domain_id);

  /**
   * Removes the data associated with a domain.
   *
   * @param int $domain_id
   */
  public static function deleteDomain($domain_id);

  /**
   * Removes the data associated with a node.
   *
   * @param int $nid
   */
  public static function deleteNode($nid);

  /**
   * Inserts a set of values into the domain_access table.
   *
   * @param array $values
   *  An array of values to be inserted. Each element is another array with the
   *  keys: nid, gid, realm.
   */
  public static function insertValues($values);
}
