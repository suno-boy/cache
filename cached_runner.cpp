#include "cached_runner.h"
#include "task_runner.h"
#include "task.h"
#include "cache.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

CachedRunner::CachedRunner(Cache& cache):sharedCache(cache){}


double CachedRunner::multiply(std::string filename) {
  std::string key = "multiply(" + filename + ")";
  double result;

  // 캐시에서 결과를 검색
  if (sharedCache.get(key, result)) {
      cache_hits++;
      return result;  // 캐시에서 결과를 찾으면, 캐시 히트를 증가시키고 결과를 반환
  } else {
      cache_misses++;
      // 캐시에 결과가 없는 경우, TaskRunner에서 계산
      result = TaskRunner::multiply(filename);
      // 계산된 결과를 캐시에 추가
      sharedCache.add(key, result);
      return result;
  }
}
int CachedRunner::palindrome(std::string filename) {
  std::string key = "palindrome(" + filename + ")";
  int result;

  // 캐시에서 결과를 검색
  if (sharedCache.get(key, result)) {
      cache_hits++;
      return result;  // 캐시에서 결과를 찾으면, 캐시 히트를 증가시키고 결과를 반환
  } else {
      cache_misses++;
      // 캐시에 결과가 없는 경우, TaskRunner에서 계산
      result = TaskRunner::palindrome(filename);
      // 계산된 결과를 캐시에 추가
      sharedCache.add(key, result);
      return result;
  }
}
int CachedRunner::hits() {
  return cache_hits;
}

int CachedRunner::misses() {
  return cache_misses;
}

// TODO: 필요한 함수 구현