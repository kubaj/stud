from base_page import BasePage
from add_coupon_page import AddCouponPage

alert_added = "Success: You have modified coupons!"

class CouponPage(BasePage):
    def __init__(self, driver):
        self.driver = driver

    def add_coupon(self):
        self.driver.find_element_by_css_selector("i.fa.fa-plus").click()
        return AddCouponPage(self.driver, self)

    def edit_coupon(self, code):
        self.driver.find_element_by_xpath("//td[text()[contains(.,'" + code + "')]]/../td[8]/a").click()
        return AddCouponPage(self.driver, self)

    def remove_coupon(self, code):
        self.driver.find_element_by_xpath("//td[text()[contains(.,'" + code + "')]]/../td[1]/input").click()
        self.driver.find_element_by_css_selector("button.btn.btn-danger").click()
        self.driver.switch_to.alert.accept()

    def is_success(self):
        return alert_added in self.driver.find_element_by_xpath("//div[contains(@class,'alert')]").text
        