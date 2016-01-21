(ns org.zproto.zproto-example-test
  (:require [clojure.test :refer :all])
  (:import [org.zproto TestZprotoExample]))

(deftest test-zproto-example
  (is (nil? (.testZprotoExample (TestZprotoExample.)))))
