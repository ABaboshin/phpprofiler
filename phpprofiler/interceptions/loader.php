<?php
print "loader.php executed\n";

// class TestInterceptor {
//     function __construct() {
//         print "TestInterceptor::__construct\n";
//     }
// }

class TestInterceptor2 {
    function __construct() {
        // print "TestInterceptor2::__construct\n";
    }

    function ExecuteBefore($data)
    {
      print "TestInterceptor2::ExecuteBefore $data[0] $data[1]\n";
    }

    function ExecuteAfter($data)
    {
      print "TestInterceptor2::ExecuteAfter $data\n";
    }

    function ExecuteException($data)
    {
      print "TestInterceptor2::ExecuteException $data\n";
    }

    function ExecuteYield($data)
    {
      print "TestInterceptor2::ExecuteYield $data\n";
    }
}

// trace_method("TestClass", "TestMethod", 'TestInterceptor');
// trace_method("TestClass", "TestMethod2", 'TestInterceptor2');

trace_method("", "testF", 'TestInterceptor2');
trace_method("", "testY", 'TestInterceptor2');

?>
