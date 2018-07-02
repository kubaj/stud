from selenium import webdriver
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.by import By
from cart_page import CartPage

class ProductPage(object):
    def __init__(self, driver: webdriver):
        self.driver = driver

    def add_to_cart(self, quantity):
        self.driver.find_element_by_id('input-quantity').clear()
        self.driver.find_element_by_id('input-quantity').send_keys(quantity)
        self.driver.find_element_by_id('button-cart').click()

    def open_cart(self):
        self.driver.find_element_by_xpath("//div[@id = 'cart']/button").click()
        wait = WebDriverWait(self.driver, 5, 1)
        wait.until(EC.element_to_be_clickable((By.XPATH, "//strong[contains(.,' View Cart')]")))
        self.driver.find_element_by_xpath("//strong[contains(.,' View Cart')]").click()
        return CartPage(self.driver)

class Product(object):
    def __init__(self, driver: webdriver, url):
        self.driver = driver
        self.url = url

    def open(self):
        self.driver.get(self.url)
        return ProductPage(self.driver)
