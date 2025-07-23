
-- stuff we use
local tostring, unpack, pairs = tostring, unpack, pairs

local oo                     = require("loop.simple")
local io                     = require("io")
local os                     = require("os")
local math                   = require("math")
local string                 = require("string")

local Applet                 = require("jive.Applet")
local System                 = require("jive.System")
local Checkbox               = require("jive.ui.Checkbox")
local Choice                 = require("jive.ui.Choice")
local Framework              = require("jive.ui.Framework")
local Icon                   = require("jive.ui.Icon")
local Label                  = require("jive.ui.Label")
local Popup                  = require("jive.ui.Popup")
local SimpleMenu             = require("jive.ui.SimpleMenu")
local Textarea               = require("jive.ui.Textarea")
local Timer                  = require("jive.ui.Timer")
local Window                 = require("jive.ui.Window")
local Networking             = require("jive.net.Networking")

local jnt                    = jnt

module(..., Framework.constants)
oo.class(_M, Applet)

local confFile = "/etc/wlan.conf"

function settingsShow(self, menuItem)
	local settingsChanged  = false
	local gonlyEnabled     = _fileMatch(confFile, "^gonly=on")
	local modegEnabled     = _fileMatch(confFile, "^mode_g=on")
	local arpwatchEnabled  = _fileMatch(confFile, "^arpwatch=on")
	local filterallEnabled = _fileMatch(confFile, "^filterall=on")
	local maxperfEnabled   = _fileMatch(confFile, "^maxperf=on")

	local window = Window("help_list", menuItem.text, 'settingstitle')
	window:setAllowScreensaver(false)

	local menu = SimpleMenu("menu")
	menu:setHeaderWidget(Textarea("help_text", self:string("WIFI_ROBUSTNESS_HELP")))

	-- Activate ARP watch - watch-arp.sh
	menu:addItem ({
		text     = self:string("ARPWATCH_ENABLE"),
		sound    = "WINDOWSHOW",
		callback = function (event, menuItem)
			local window = Window("text_list", menuItem.text)
			window:setAllowScreensaver(false)
			local menu = SimpleMenu("menu")
			menu:setHeaderWidget(Textarea("help_text", self:string("ARPWATCH_HOWTO")))
			local checkb = Checkbox("checkbox",
					function(_, isSelected)
						settingsChanged = true
						if isSelected then
							log:info("wlan.conf setting arpwatch=on");
							_fileSub(confFile, "^arpwatch=.*$", "arpwatch=on")
							arpwatchEnabled = true
						else
							log:info("wlan.conf setting arpwatch=off");
							_fileSub(confFile, "^arpwatch=.*$", "arpwatch=off")
							arpwatchEnabled = false
						end
					end,
					arpwatchEnabled
				)
			menu:addItem({
				text  = menuItem.text,
				style = 'item_choice',
				check = checkb,
			})
			window:addWidget(menu)
			self:tieAndShowWindow(window)
		end
	})

	-- WiFi mode
	-- Enable setting one of 'wmiconfig -i eth1 --wmode gonly' or 'wmiconfig -i eth1 --wmode g'
	menu:addItem ({
		text     = self:string("WMODE_SETTINGS"),
		sound    = "WINDOWSHOW",
		callback = function (event, menuItem)
			local window = Window("text_list", menuItem.text)
			window:setAllowScreensaver(false)

			-- Set up current choice
			local currentIndex = 1  -- Off
			if gonlyEnabled then
				currentIndex = 2
			elseif modegEnabled then
				currentIndex = 3
			end

			local menu = SimpleMenu("menu")
			menu:setHeaderWidget(Textarea("help_text", self:string("WMODE_HOWTO")))

			menu:addItem ({
				text     = self:string("WMODE_HELP"),
				sound    = "WINDOWSHOW",
				callback = function (event, menuItem)
					local window = Window("text_list", menuItem.text)
					window:setAllowScreensaver(false)
					local text = Textarea('help_text', self:string("WMODE_HELPTXT"))
					window:addWidget(text)
					self:tieAndShowWindow(window)
				end
			})

			local choices = Choice(
					"choice",
					{ "Off - recommended", "802.11b disabled", "'mode g'" },
					function(obj, selectedIndex)
						log:debug( "Choice updated: ", tostring(selectedIndex), " - ", tostring(obj:getSelected()) )
						settingsChanged = true
						if selectedIndex == 2 then
							gonlyEnabled = true
							modegEnabled = false
							log:info("wlan.conf setting gonly=on");
							_fileSub(confFile, "^gonly=.*$", "gonly=on")
							_fileSub(confFile, "^mode_g=.*$", "mode_g=off")
						elseif selectedIndex == 3 then
							modegEnabled = true
							gonlyEnabled = false
							log:info("wlan.conf setting mode_g=on");
							_fileSub(confFile, "^mode_g=.*$", "mode_g=on")
							_fileSub(confFile, "^gonly=.*$", "gonly=off")
						else -- default
							gonlyEnabled = false
							modegEnabled = false
							log:info("wlan.conf setting gonly & mode_g=off");
							_fileSub(confFile, "^gonly=.*$", "gonly=off")
							_fileSub(confFile, "^mode_g=.*$", "mode_g=off")
						end
						currentIndex = selectedIndex
					end,
					currentIndex
				)
			menu:addItem({
				text  = self:string("WMODE_LABEL"),
				style = 'item_choice',
				check = choices,
			})
			window:addWidget(menu)
			self:tieAndShowWindow(window)
		end
	})

	-- Enable setting 'wmiconfig -i eth1 --filter=all'
	menu:addItem ({
		text     = self:string("FILTERALL_ENABLE"),
		sound    = "WINDOWSHOW",
		callback = function (event, menuItem)
			local window = Window("text_list", menuItem.text)
			window:setAllowScreensaver(false)
			local menu = SimpleMenu("menu")
			menu:setHeaderWidget(Textarea("help_text", self:string("FILTERALL_HOWTO")))
			local checkb = Checkbox("checkbox",
					function(_, isSelected)
						settingsChanged = true
						if isSelected then
							log:info("wlan.conf setting filterall=on")
							_fileSub(confFile, "^filterall=.*$", "filterall=on")
							filterallEnabled = true
						else
							log:info("wlan.conf setting filterall=off")
							_fileSub(confFile, "^filterall=.*$", "filterall=off")
							filterallEnabled = false
						end
					end,
					filterallEnabled
				)
			menu:addItem({
				text  = menuItem.text,
				style = 'item_choice',
				check = checkb,
			})
			window:addWidget(menu)
			self:tieAndShowWindow(window)
		end
	})

	-- Displays the number of truncated beacons logged.
	menu:addItem ({
		text     = self:string("TRUNCATED_BCN_TITLE"),
		sound    = "WINDOWSHOW",
		callback = function (event, menuItem)
			local window = Window("text_list", menuItem.text)
			window:setAllowScreensaver(false)
			local grepRes = io.popen("/bin/grep -ci \'AR6000\\s\\+Truncated\' /var/log/messages")
			local truncation_cnt = grepRes:read("*line")
			grepRes:close()
			if not truncation_cnt then
				truncation_cnt = "<Read error>"
			end
			local text = Textarea('help_text', self:string("TRUNCATED_BCN_TEXT", tostring(truncation_cnt)))
			window:addWidget(text)
			self:tieAndShowWindow(window)
		end
	})

	-- Enable setting 'wmiconfig -i eth1 --power maxperf'
	menu:addItem ({
		text     = self:string("MAXPERF_ENABLE"),
		sound    = "WINDOWSHOW",
		callback = function (event, menuItem)
			local window = Window("text_list", menuItem.text)
			window:setAllowScreensaver(false)
			local menu = SimpleMenu("menu")
			menu:setHeaderWidget(Textarea("help_text", self:string("MAXPERF_HOWTO")))
			local checkb = Checkbox("checkbox",
					function(_, isSelected)
						settingsChanged = true
						if isSelected then
							log:info("wlan.conf setting maxperf=on")
							_fileSub(confFile, "^maxperf=.*$", "maxperf=on")
							maxperfEnabled = true
						else
							log:info("wlan.conf setting maxperf=off")
							_fileSub(confFile, "^maxperf=.*$", "maxperf=off")
							maxperfEnabled = false
						end
					end,
					maxperfEnabled
				)
			menu:addItem({
				text  = menuItem.text,
				style = 'item_choice',
				check = checkb,
			})
			window:addWidget(menu)
			self:tieAndShowWindow(window)
		end
	})

	window:addWidget(menu)

	-- Restart the WiFi when the menu is exited
	window:addListener(EVENT_WINDOW_POP,
		function()
			if settingsChanged then
				log:info("Executing /lib/atheros/restart-wifi.sh")
				os.execute("/lib/atheros/restart-wifi.sh &")
			end
		end
	)

	self.window = window
	self.menu   = menu

	self:tieAndShowWindow(window)
	return window
end

function _fileMatch(file, pattern)
	local fi, err = io.open(file, "r")

	if (err) then 
		return false
	end

	for line in fi:lines() do
		if string.match(line, pattern) then
			fi:close()
			return true
		end
	end
	fi:close()

	return false
end

function _fileSub(file, pattern, repl)
	local data = ""
	local match = false

	local fi, errmsg = io.open(file, "r")
	if fi ~= nil then
		for line in fi:lines() do
			if string.match(line, pattern) then
				match = true
				line = string.gsub(line, pattern, repl)
			end
			data = data .. line .. "\n"
		end
		fi:close()
	else
		-- user deleted file ?
		-- we will create one anyway, with the current setting
		log:warn("Error reading ", file, " : ", errmsg)
	end
	-- if we haven't found a match to replace, add to end of file
	if match == false then
		data = data .. repl .. "\n"
	end
	System:atomicWrite(file, data)
end

--[[

=head1 LICENSE

This file is licensed under BSD. Please see the LICENSE file for details.

=cut
--]]
