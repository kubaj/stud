# Test cases (attachment to Test Plan)

Tests should begin on url `http://mys01.fit.vutbr.cz:8030/admin/index.php?route=marketing/coupon`. Access token is needed for acessing admin board of the OpenCart.

## Creating deactivated coupon
1. Click on 'Add' button
2. Fill these values to input fields:
    * Coupon Name: "Test Coupon"
    * Code: "tcode1"
    * Discount: "10"
    * Status: "Disabled"
    * Other input fields are default
3. Click save button
4. Page should be redirected to coupons overview and coupon "Test Coupon" should be visible

## Adding non-existing coupon to cart
1. Visit item's [url](http://mys01.fit.vutbr.cz:8030/index.php?route=product/product&path=20_27&product_id=41).
2. Add item "iMac" to cart.
3. Click on cart and click "View cart" link inside box.
4. Select option "Use coupon code", insert code "notacode" into the input field and click "Apply coupon" button.
5. Warning about invalidity of the code should be shown.

## Adding deactivated coupon to cart
1. Visit item's [url](http://mys01.fit.vutbr.cz:8030/index.php?route=product/product&path=20_27&product_id=41).
2. Click on cart and click "View cart" link inside box.
3. Select option "Use coupon code" and insert code "tcode1" into the input field and click "Apply coupon" button.
4. Warning about invalidity of the code should be shown.

## Coupon with future date 
1. Click "Edit" button of the code "Test coupon" and change "Status" of the code to "Enabled".
2. Select tommorow date for start date.
3. Click "Save" button. 
4. Visit item's [url](http://mys01.fit.vutbr.cz:8030/index.php?route=product/product&path=20_27&product_id=41).
5. Click on cart and click "View cart" link inside box.
6. Select option "Use coupon code" and insert code "tcode1" into the input field and click "Apply coupon" button.
7. Warning about invalidity of the code should be shown.

## Coupon for category
1. Click "Edit" button of the code "Test coupon" 
2. Select today date for start date.
3. Add category "Components" to Coupon Category.
4. Click "Save" button. 
5. Visit second's item [url](mys01.fit.vutbr.cz:8030/index.php?route=product/product&path=25_28&product_id=33).
6. Click on cart and click "View cart" link inside box.
7. Select option "Use coupon code" and insert code "tcode1" into the input field and click "Apply coupon" button.
8. In price summary should be row "Coupon (tcode1)" with minus value of the 10% of the "Samsung SyncMaster" price.


## Activating coupon and adding coupon to cart again
1. Click button "Checkout". Select option "Guest checkout" and click button "Continue".
2. Fill values and click "Continue" button: 
    * First Name: "Testo"
    * Last Name: "Tester"
    * E-mail: "sample@sample.com"
    * Address 1: Address 1
    * City: City
    * Post Code: 12345
    * Country: "Czech Republic"
    * Region / State: "Jihomoravsky"
3. Click again "Continue"
4. Check "I have read and agree to the Terms & Conditions" and click "Continue" button.
5. Click "Confirm order" button.
6. There should be message "Your order has been placed!".
7. Visit [coupon administration](http://mys01.fit.vutbr.cz:8030/admin/index.php?route=marketing/coupon) and click "Edit" of the "Test coupon", then click "History" where should be 1 order.

## Coupon usage restriction
1. Visit item's [url](http://mys01.fit.vutbr.cz:8030/index.php?route=product/product&path=20_27&product_id=41).
2. Add item "iMac" to cart.
3. Click on cart and click "View cart" link inside box.
4. Select option "Use coupon code", insert code "notacode" into the input field and click "Apply coupon" button.
5. Warning about invalidity of the code should be shown.

## Remove coupon
1. Select checkbox of the "Test coupon" coupon and click "Remove" button.
2. Coupon "Test coupon" shouldn't be in the list of the coupons.