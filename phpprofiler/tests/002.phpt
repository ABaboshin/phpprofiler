--TEST--
phpprofiler_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('phpprofiler')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = phpprofiler_test1();

var_dump($ret);
?>
--EXPECT--
The extension phpprofiler is loaded and working!
NULL
