<?php

/**
 * Implementation of a domain access service which stores the data in a cache.
 *
 * This does not make sense to be used when the cache is stored in the database,
 * but only when using a faster cache, like an in-memory cache (memcache, redis,
 * etc.)
 */
class DomainAccessCache extends DomainAccess {

  /**
   * {@inheritdoc}
   */
  public static function reassignDomain($old_domain_id, $new_domain_id) {
    parent::reassignDomain($old_domain_id, $new_domain_id);
    // Sadly, we need to empty the entire cache because we don't really know
    // which nodes were affected at this point.
    cache_clear_all('*', 'cache_domain_access', TRUE);
  }

  /**
   * {@inheritdoc}
   */
  public static function deleteDomain($domain_id) {
    parent::deleteDomain($domain_id);
    cache_clear_all('*', 'cache_domain_access', TRUE);
  }

  /**
   * {@inheritdoc}
   */
  public static function deleteNode($nid) {
    parent::deleteNode($nid);
    cache_clear_all($nid, 'cache_domain_access');
  }

  /**
   * {@inheritdoc}
   */
  protected static function getByNid($nid) {
    // Try to get the data from cache first.
    if ($cache = cache_get($nid, 'cache_domain_access')) {
      return $cache->data;
    }
    // If not found in cache, fallback to the parent implementation and then
    // store it into cache.
    $data = parent::getByNid($nid);
    cache_set($nid, $data, 'cache_domain_access');
    return $data;
  }
}
