from selenium import webdriver
from base_page import BasePage
from coupon_page import CouponPage

class DashboardPage(BasePage):
    def __init__(self, driver: webdriver):
        self.driver = driver
        
    def open_coupons(self):
        self.driver.find_element_by_xpath("//a[@class='pull-left']").click()
        elem = self.driver.find_element_by_xpath("//*[@id=\"menu\"]/li[7]")
        elem.click()
        self.driver.find_element_by_link_text("Coupons").click()
        return CouponPage(self.driver)