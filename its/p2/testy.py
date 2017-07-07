# -*- coding: utf-8 -*-
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support.ui import WebDriverWait
from selenium.common.exceptions import *
from selenium.webdriver.support import expected_conditions as EC
import unittest, time, re
from login_page import LoginPage
from add_coupon_page import CouponSetting
from product_page import Product,ProductPage
from decimal import Decimal
import test_utils as tu

from selenium.webdriver.common.desired_capabilities import DesiredCapabilities

class CouponTest(unittest.TestCase):
    def setUp(self):
        # self.driver = webdriver.Chrome()
        self.driver = webdriver.Remote(
                    command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',
                    desired_capabilities=DesiredCapabilities.CHROME)
        self.driver.implicitly_wait(30)
        self.base_url = "http://mys01.fit.vutbr.cz:8030/"
        self.verificationErrors = []
        self.accept_next_alert = True        
    
    def test_adding_nonexisting_coupon(self):
        driver = self.driver
        product = Product(driver, self.base_url + "/index.php?route=product/product&path=20_27&product_id=41")
        p_product = product.open()
        p_product.add_to_cart("1")
        p_cart = p_product.open_cart()

        p_cart.insert_coupon("notacode")
        self.assertTrue(p_cart.is_coupon_invalid())
        
    def test_adding_deactivated_coupon(self):
        driver = self.driver
        product = Product(driver, self.base_url + "/index.php?route=product/product&path=20_27&product_id=41")
        p_product = product.open()
        p_product.add_to_cart("1")
        p_cart = p_product.open_cart()

        p_cart.insert_coupon("2222")
        self.assertTrue(p_cart.is_coupon_invalid())
  
    def test_using_coupon_future_date(self):
        driver = self.driver
        driver.get(self.base_url + 'admin')

        product = Product(driver, self.base_url + "/index.php?route=product/product&path=20_27&product_id=41")

        coupon_setting = CouponSetting()
        coupon_setting.add('name', 'Future date')
        coupon_setting.add('code', 'future')
        coupon_setting.add('start_date', tu.get_tommorow_date())

        loginp = LoginPage(self.driver)
        dashboardp = loginp.login('admin', 'admin')
        couponp = dashboardp.open_coupons()

        add_couponp = couponp.add_coupon()
        add_couponp.fill_option(coupon_setting)
        couponp = add_couponp.add()
        self.assertTrue(couponp.is_success())

        p_product = product.open()
        p_product.add_to_cart("1")
        p_cart = p_product.open_cart()

        p_cart.insert_coupon(coupon_setting.get('code'))
        self.assertTrue(p_cart.is_coupon_invalid())

    def test_adding_category_restricted_coupon(self):
        driver = self.driver
        driver.get(self.base_url + "/admin/")

        product = Product(driver, self.base_url + "/index.php?route=product/product&product_id=41")
        product2 = Product(driver, self.base_url + "/index.php?route=product/product&product_id=33")

        coupon_setting = CouponSetting()
        coupon_setting.add('name', 'Desktop')
        coupon_setting.add('code', 'desktop')
        coupon_setting.add('category','Desktops')
        coupon_setting.add('discount','10')

        loginp = LoginPage(self.driver)
        dashboardp = loginp.login('admin', 'admin')
        couponp = dashboardp.open_coupons()

        add_couponp = couponp.add_coupon()
        add_couponp.fill_option(coupon_setting)
        couponp = add_couponp.add()
        self.assertTrue(couponp.is_success())

        productp = product.open()
        productp.add_to_cart(1)
        productp = product2.open()
        productp.add_to_cart(1)

        cartp = productp.open_cart()
        cartp.insert_coupon(coupon_setting.get('code'))
        self.assertTrue(cartp.is_coupon_valid())

        product_price = cartp.price_by_name("iMac")
        discount_price = cartp.get_coupon_discount()

        self.assertEqual(-product_price/10, discount_price)
        

    def test_activating_coupon(self):
        driver = self.driver
        driver.get(self.base_url + 'admin')

        product = Product(driver, self.base_url + "/index.php?route=product/product&path=20_27&product_id=41")

        coupon_setting = CouponSetting()
        coupon_setting.add('code', '1111')
        coupon_setting.add('enabled', 'Enabled')

        loginp = LoginPage(self.driver)
        dashboardp = loginp.login('admin', 'admin')
        couponp = dashboardp.open_coupons()

        add_couponp = couponp.edit_coupon(coupon_setting.get('code'))
        add_couponp.fill_option(coupon_setting)
        couponp = add_couponp.add()
        self.assertTrue(couponp.is_success())

        p_product = product.open()
        p_product.add_to_cart("1")
        p_cart = p_product.open_cart()

        p_cart.insert_coupon(coupon_setting.get('code'))
        self.assertTrue(p_cart.is_coupon_valid())
        
    def test_adding_coupon_usage_restriction(self):
        driver = self.driver
        driver.get(self.base_url + 'admin')

        product = Product(driver, self.base_url + "/index.php?route=product/product&path=20_27&product_id=41")

        coupon_setting = CouponSetting()
        coupon_setting.add('name', 'Usage restricted')
        coupon_setting.add('code', 'usagerestr')
        coupon_setting.add('discount', '10')

        loginp = LoginPage(self.driver)
        dashboardp = loginp.login('admin', 'admin')
        couponp = dashboardp.open_coupons()

        add_couponp = couponp.add_coupon()
        add_couponp.fill_option(coupon_setting)
        couponp = add_couponp.add()
        self.assertTrue(couponp.is_success())

        p_product = product.open()
        p_product.add_to_cart("1")
        p_cart = p_product.open_cart()

        p_cart.insert_coupon(coupon_setting.get('code'))
        self.assertTrue(p_cart.is_coupon_valid())

        p_cart.checkout()

        p_product = product.open()        
        p_product.add_to_cart("1")
        p_cart = p_product.open_cart()

        p_cart.insert_coupon(coupon_setting.get('code'))
        self.assertTrue(p_cart.is_coupon_invalid())
        
    def test_remove_coupon(self):
        driver = self.driver
        driver.get(self.base_url + "admin")

        coupon_setting = CouponSetting()
        coupon_setting.add('code', '3333')

        loginp = LoginPage(self.driver)
        dashboardp = loginp.login('admin', 'admin')
        couponp = dashboardp.open_coupons()

        couponp.remove_coupon(coupon_setting.get('code'))
        self.assertTrue(couponp.is_success())
        
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        self.driver.get('http://mys01.fit.vutbr.cz:5000/reset/30')
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)

if __name__ == "__main__":
    unittest.main()
