<?php

class TestClass {
    function __construct() {
        print "Create TestClass\n";
    }
}

trace_method("TestClass", "TestMethod", TestClass::class);
// $ch = curl_init();
// curl_setopt($ch, CURLOPT_URL, "google.de");
// curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
// $output = curl_exec($ch);
// curl_close($ch);
// print $output;
?>
