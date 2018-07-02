# Test report of system OpenCart
 * Tested module: Coupon module
 * Author: Jakub Kulich (xkulic03)

## Bugs
There was 1 possible bug found during testing

## Category restriction of coupon is not working
Documentation of OpenCart says: 
> Selecting a product category will apply the discount to all products within that category.

Now OpenCart will apply discount to all items in cart if one the items of coupon's category is in cart.

#### Reproducing steps
1. Add coupon, where discount is 10%, and add category "Desktops" to list of categories.
2. Add one item from category "Desktops" and one item from another category.
3. Open cart -> Click apply coupon and insert created coupon's code.
4. Coupon discount value now should be 10% of the item from "Desktops" category. Instead, there is wrong value and that is 10% of all items in cart.