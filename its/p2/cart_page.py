from selenium import webdriver
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import Select
import test_utils as tu
import time

coupon_invalid = "Warning: Coupon is either invalid, expired or reached its usage limit!"
coupon_valid = "Success: Your coupon discount has been applied!"

class CartPage(object):
    def __init__(self, driver: webdriver):
        self.driver = driver

    def insert_coupon(self, code):
        self.driver
        self.driver.find_element_by_link_text("Use Coupon Code").click()      
        self.driver.find_element_by_id("input-coupon").send_keys(code)
        self.driver.find_element_by_id("button-coupon").click()

    def price_by_name(self, name):
        price = self.driver.find_element_by_xpath("//td/a[text()[contains(.,'" + name + "')]]/../../td[6]").text
        return tu.get_value_from_price(price)
        self.driver.find_element_by_id("button-coupon").click()

    def get_coupon_discount(self):
        discount = self.driver.find_elements_by_xpath("//td/strong[text()[contains(.,'Coupon')]]/../../td[2]")[1].text
        return tu.get_value_from_price(discount)
        
    def is_coupon_invalid(self):
        return coupon_invalid in self.driver.find_element_by_xpath("//div[contains(@class,'alert')]").text

    def is_coupon_valid(self):
        return coupon_valid in self.driver.find_element_by_xpath("//div[contains(@class,'alert')]").text

    def checkout(self):
        self.driver.find_element_by_css_selector("a.btn.btn-primary").click()
        time.sleep(1)
        self.driver.find_element_by_xpath("(//input[@name='account'])[2]").click()
        self.driver.find_element_by_id("button-account").click()
        self.driver.find_element_by_id("input-payment-firstname").clear()
        self.driver.find_element_by_id("input-payment-firstname").send_keys("Testo")
        self.driver.find_element_by_id("input-payment-lastname").clear()
        self.driver.find_element_by_id("input-payment-lastname").send_keys("Tester")
        self.driver.find_element_by_id("input-payment-email").clear()
        self.driver.find_element_by_id("input-payment-email").send_keys("sample@sample.com")
        self.driver.find_element_by_id("input-payment-telephone").clear()
        self.driver.find_element_by_id("input-payment-telephone").send_keys("123456789")
        self.driver.find_element_by_id("input-payment-address-1").clear()
        self.driver.find_element_by_id("input-payment-address-1").send_keys("Address 1")
        self.driver.find_element_by_id("input-payment-city").clear()
        self.driver.find_element_by_id("input-payment-city").send_keys("City")
        self.driver.find_element_by_id("input-payment-postcode").clear()
        self.driver.find_element_by_id("input-payment-postcode").send_keys("12345")
        Select(self.driver.find_element_by_id("input-payment-country")).select_by_visible_text("Czech Republic")
        self.driver.find_element_by_css_selector("option[value=\"56\"]").click()
        Select(self.driver.find_element_by_id("input-payment-zone")).select_by_visible_text(u"Jihomoravský")
        self.driver.find_element_by_id("button-guest").click()
        time.sleep(1)
        self.driver.find_element_by_id("button-shipping-method").click()
        time.sleep(1)
        self.driver.find_element_by_name("agree").click()
        self.driver.find_element_by_id("button-payment-method").click()
        time.sleep(1)
        self.driver.find_element_by_id("button-confirm").click()
        time.sleep(1)