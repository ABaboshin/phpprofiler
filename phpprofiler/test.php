<?php

class TestClass {
    function __construct() {
        print "TestClass::__construct\n";
    }

    public function TestMethod()
    {
      print "TestClass::TestMethod\n";
    }

    public function test()
    {
      print "TestClass::test\n";
    }
}

function testF($i, $str)
{
  if ($i > 0) testF($i - 1, $str);
  print "testF $i\n";
  return $i;
}

// $test = new TestClass();
// $test->TestMethod();

testF(1, "test");

// trace_method("TestClass", "TestMethod", TestClass::class);
// trace_method("", "curl_exec", TestClass::class);
$ch = curl_init();
// curl_setopt($ch, CURLOPT_URL, "google.de");
// curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
// $output = curl_exec($ch);
// curl_close($ch);
// print $output;

print "done\n";
?>
