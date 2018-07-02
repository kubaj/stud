from selenium import webdriver

class BasePage(object):
    def __init__(self, driver: webdriver):
        self.driver = driver
        
    def logout(self):
        driver.find_element_by_link_text("Logout").click()
