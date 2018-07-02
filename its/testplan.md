# Test plan of OpenCart for course ITS

## Test Plan Identifier:
  
Test plan of OpenCart

## Introduction:

This plan covers basic funcionality of the Coupons module on GUI level.

## References:

 * [Test plan (this document)](testplan.md)
 * [Test cases](testcases.md)

## Test Items:

Tested product is an open source PHP-based online e-commerce solution OpenCart. Version of OpenCart is 2.2.0.0.

## Features to be Tested:

There is one feature selected for testing:
 * Coupons module

## Features Not to Be Tested:

Untested features are all except those that are mentioned above as tested. Testing of all features is not intention of this project.

## Approach:

  * Testing type is integration testing on GUI level.
  * Testing method is automated testing using tool Selenium.

## Suspension Criteria and Resumption Requirements:

All tests should have pass if there is connection to the webserver.

## Test Environment:

For testing will be used Selenium - tool automating web applications for testing purposes. Selenium needs web browser to work (List of browsers is in [documentation](http://www.seleniumhq.org/docs/01_introducing_selenium.jsp). Selenium tests will be run on instance of OpenCart provided for this project. We can consider running local instance of OpenCart in Docker in case of unavailability of provided instance.

## Staffing and Training Needs:

Tester should have basic knowledge of web technologies such as HTML and CSS.

## Risks:

There are few risks:
 * Angry customers if given coupon is not working
 * Money loss if coupon with restricted usage is used more times than intended