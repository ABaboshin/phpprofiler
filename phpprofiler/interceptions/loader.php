<?php
print "loader.php executed\n";

class TestInterceptor {
    function __construct() {
        print "TestInterceptor::__construct\n";
    }
}

trace_method("TestClass", "TestMethod", 'TestInterceptor');

?>
