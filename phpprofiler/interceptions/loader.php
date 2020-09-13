<?php
print "loader.php executed\n";

class TestInterceptor {
    function __construct() {
        print "TestInterceptor::__construct\n";
    }
}

class TestInterceptor2 {
    function __construct() {
        print "TestInterceptor2::__construct\n";
    }
}

trace_method("TestClass", "TestMethod", 'TestInterceptor');

trace_method("", "testF", 'TestInterceptor2');

?>
