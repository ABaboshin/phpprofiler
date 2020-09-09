--TEST--
Check if phpprofiler is loaded
--SKIPIF--
<?php
if (!extension_loaded('phpprofiler')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "phpprofiler" is available';
?>
--EXPECT--
The extension "phpprofiler" is available
