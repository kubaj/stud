from selenium import webdriver
from selenium.webdriver.support.ui import Select
from base_page import BasePage

class AddCouponPage(BasePage):
    def __init__(self, driver: webdriver, parent):
        self.driver = driver
        self.parent = parent

    def fill_option(self, option):
        if option.get('name') != None:
            self.driver.find_element_by_id("input-name").clear()
            self.driver.find_element_by_id("input-name").send_keys(option.get('name'))
        
        if option.get('code') != None:
            self.driver.find_element_by_id("input-code").clear()
            self.driver.find_element_by_id("input-code").send_keys(option.get('code'))

        if option.get('start_date') != None:
            self.driver.find_element_by_id("input-date-start").clear()
            self.driver.find_element_by_id("input-date-start").send_keys(option.get('start_date'))

        if option.get('discount') != None:
            self.driver.find_element_by_id("input-discount").clear()
            self.driver.find_element_by_id("input-discount").send_keys(option.get('discount'))

        if option.get('usages') != None:
            self.driver.find_element_by_id("input-uses-total").clear()
            self.driver.find_element_by_id("input-uses-total").send_keys(option.get('usages'))
        
        if option.get('category') != None:
            self.driver.find_element_by_id("input-category").clear()
            self.driver.find_element_by_id("input-category").send_keys(option.get('category'))
            self.driver.find_element_by_link_text(option.get('category')).click()
        
        if option.get('enabled') != None:
            Select(self.driver.find_element_by_id("input-status")).select_by_visible_text(option.get('enabled'))

    def add(self):
        self.driver.find_element_by_css_selector("button.btn.btn-primary").click()
        return self.parent

class CouponSetting(object):
    def __init__(self):
        self.options = dict()
    def add(self, option, value):
        self.options[option] = value
    def get(self, option):
        if not option in self.options:
            return None
        return self.options[option]