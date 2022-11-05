-- DB update 2022_11_01_04 -> 2022_11_01_05

-- Fixed spawntime
UPDATE `creature` SET `spawntimesecs`= 60 WHERE  `guid`=66716;

-- Add SmartAI to insert the phrase
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 18548;

DELETE FROM `smart_scripts` WHERE (`entryorguid` = 18548) AND (`source_type` = 0) AND (`id` IN (0, 1));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(18548, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Firewing Courier - In Combat - Say Line 0');

