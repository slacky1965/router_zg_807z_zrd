import * as m from 'zigbee-herdsman-converters/lib/modernExtend';
import * as exposes_1 from 'zigbee-herdsman-converters/lib/exposes';
import * as reporting from 'zigbee-herdsman-converters/lib/reporting';
import * as utils_1 from 'zigbee-herdsman-converters/lib/utils';
import * as logger from 'zigbee-herdsman-converters/lib/logger';


export default {
        zigbeeModel: ["Router-ZG-807Z-SlD"],
        model: "Router-ZG-807Z-SlD",
        vendor: "Slacky-DIY",
        description: "Tuya router with custom firmware",
        extend: [
            m.deviceAddCustomCluster("SlackyDiyCustomCluster", {
                name: "SlackyDiyCustomCluster",
                ID: 0xff65,
                manufacturerCode: 0x6565,
                attributes: {
                    txPower: {
                        name: "txPower",
                        ID: 0x0000,
                        type: 0x30,
                        write: true,
                        max: 0xff,
                    },
                },
                commands: {},
                commandsResponse: {},
            }),
            m.battery({
                voltage: true,
                voltageReporting: true,
                lowStatus: true,
                percentageReportingConfig: {min: 5, max: 3600, change: 0},
                voltageReportingConfig: {min: 5, max: 3600, change: 0},
                lowStatusReportingConfig: {min: 0, max: 3600, change: 0},
            }),
            m.numeric({
                name: "uptime_time",
                access: "STATE_GET",
                cluster: "genTime",
                attribute: "time",
                reporting: {min: 60, max: 3600, change: 15},
                unit: "sec",
                description: "Uptime of device",
            }),
            m.enumLookup({
                name: "tx_power",
                cluster: "SlackyDiyCustomCluster",
                attribute: "txPower",
                lookup: {"0 dBm": 0, "5 dBm": 1, "10 dBm": 2},
                reporting: false,
                description: "Rf power config",
            }),
            m.enumLookup({
                name: "switch_actions",
                lookup: {on_off: 0, off_on: 1, toggle: 2},
                cluster: "genOnOffSwitchCfg",
                attribute: "switchActions",
                description: "Actions switch",
            }),
        ],
        ota: true,
};
