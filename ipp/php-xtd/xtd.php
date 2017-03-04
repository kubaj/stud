#!/usr/bin/php
<?php

##
## xkulic03 (Jakub Kulich)
##

mb_internal_encoding('UTF-8');

$parameters = handle_parameters($argv);

$output_stream = STDOUT;
$input_stream = STDIN;

if (isset($parameters["input"])) {
    if (($input_stream = fopen($parameters["input"], "r")) === false)
        panic("Input file error", 2);
}

if (isset($parameters["output"])) {
    if (($output_stream = fopen($parameters["output"], "w")) === false)
        panic("Output file error", 3);
}

if (array_key_exists("help", $parameters))
    print_help();

if (isset($parameters["header"])) {
    fprintf($output_stream, "--%s\n\n", $parameters["header"]);
}

if (!($xml = simplexml_load_string(stream_get_contents($input_stream)))) {
    panic("XML is empty or root empty.", 0);
}

if (isset($parameters["isvalid"])) {
    $xmlv = simplexml_load_string(file_get_contents($parameters["isvalid"]));

    $relations = get_relations($xml, array());
    $relationsv = get_relations($xmlv, array());

    $tables = get_tables(xml_to_array($xml), array());
    $tablesv = get_tables(xml_to_array($xmlv), array());

    if (validate($tables, $tablesv) == false)
        panic("XML is invalid", 91);
} else {
    $relations = get_relations($xml, array());
    $tables = get_tables(xml_to_array($xml), array());
}


if (array_key_exists("g", $parameters)) {
    fprintf($output_stream, "%s", relationships_table($tables, $relations, $parameters));
} else {
    fprintf($output_stream, "%s", create_tables($tables, $relations, $parameters));
}

fclose($input_stream);
fclose($output_stream);

/**
 * @param $tables array of tables of validation xml
 * @param $tablesv array of tables of validated xml
 *
 * @return bool true if valid
 */
function validate($tables, $tablesv)
{
    foreach ($tablesv as $key => $value) {
        if (count($tablesv[$key]) > count($tables[$key]))
            return false;

        if (!isset($tables[$key]))
            return false;

        if (count($tablesv[$key]["attributes"]) > count($tables[$key]["attributes"]))
            return false;

        if (count($tablesv[$key]["keys"]) > count($tables[$key]["keys"]))
            return false;

        foreach ($tablesv[$key]["attributes"] as $attr_key => $attr_value) {
            if (!isset($tables[$key]["attributes"][$attr_key]))
                return false;
        }

        if (!array_key_exists("value", $tables[$key])
            && array_key_exists("value", $tablesv[$key])
        ) {
            return false;
        }

        if ($tablesv[$key]["value"] != $tables[$key]["value"])
            return false;
    }

    return true;
}


/**
 * Function generates tables of attributes
 *
 * @param $array array xml
 * @param $tables array of tables
 * @param string $old parent key
 * @return mixed array of tables
 */
function get_tables($array, $tables, $old = "")
{
    foreach ($array as $key => $value) {
        if (is_string($key)) {
            $key = mb_strtolower($key, 'UTF-8');
            if (is_array($value)) {
                if ($key == "@attr")
                    set_attributes($tables[$old]["attributes"], $value);
                else {
                    if (!array_key_exists($key, $tables))
                        $tables[$key] = array("attributes" => array(), "keys" => array());

                    $old = $key;
                }
            } else if ($key == "@val") {
                if (isset($tables[$old]["value"])) {
                    $tables[$old]["value"] = data_type_priority($tables[$old]["value"], get_data_type($value));
                } else {
                    $tables[$old]["value"] = get_data_type($value);
                }
            }
        }

        if (is_array($value)) {
            $tables = get_tables($value, $tables, $old);
        }
    }

    return $tables;
}

/**
 * Function sets attributes to table
 *
 * @param $table
 * @param $attributes
 */
function set_attributes(&$table, $attributes)
{
    foreach ($attributes as $key => $value) {
        $key = mb_strtolower($key, 'UTF-8');

        if (isset($table[$key]))
            $table[$key] = data_type_priority($table[$key], get_data_type($value));
        else
            $table[$key] = get_data_type($value);

        if ($table[$key] == 4)
            $table[$key] = 3;
    }
}

/**
 * Function generates table of relations from xml array
 *
 * @param $xml_object object xml
 * @param $relations array of relations
 * @param string $parent parent key
 * @return mixed array of relations
 */
function get_relations($xml_object, $relations, $parent = "")
{
    $parent = mb_strtolower($parent, 'UTF-8');

    $count_array = array();

    foreach ($xml_object as $key => $value) {
        $key = mb_strtolower($key, 'UTF-8');

        if (!array_key_exists($key, $relations))
            $relations[$key] = array();

        if (!empty($parent)) {
            if (array_key_exists($key, $count_array))
                $count_array[$key]++;
            else
                $count_array[$key] = 1;
        }

        $relations = get_relations($value, $relations, $key);
    }

    foreach ($count_array as $child => $value) {
        if (!empty($parent) && (!array_key_exists($child, $relations[$parent]) || $relations[$parent][$child] < $count_array[$child]))
            $relations[$parent][$child] = $count_array[$child];
    }

    return $relations;
}

/**
 * Function create set of ddl commands to create tables
 *
 * @param $tables array of tables
 * @param $relations array of relations
 * @param $parameters array of program parameters
 * @return string
 */
function create_tables($tables, $relations, $parameters)
{
    $out = "";

    foreach ($tables as $key => $value) {
        $key = mb_strtolower($key, 'UTF-8');


        $primary_key = "prk_" . $key . "_id";
        $out .= "CREATE TABLE " . $key . " (\n\t" . $primary_key . " INT PRIMARY KEY";

        if (array_key_exists($primary_key, $value["keys"]) || array_key_exists($primary_key, $value["attributes"]))
            panic("Conflict of keys", 90);

        foreach ($relations[$key] as $key_attr => $attribute) {
            $key_attr = mb_strtolower($key_attr, 'UTF-8');


            if (array_key_exists("b", $parameters)) {
                $key_attr .= "_id";

                if (array_key_exists($key_attr, $value["keys"]) || array_key_exists($key_attr, $value["attributes"]))
                    panic("Conflict of keys", 90);

                $out .= ",\n\t" . $key_attr . " INT";
            } else if (!array_key_exists("etc", $parameters) || $parameters["etc"] >= $attribute) {
                for ($i = 1; $i <= $attribute; $i++) {
                    if ($attribute > 1)
                        $atr_name = $key_attr . $i . "_id";
                    else
                        $atr_name = $key_attr . "_id";

                    if (array_key_exists($atr_name, $value["keys"]) || array_key_exists($atr_name, $value["attributes"]))
                        panic("Conflict of keys", 90);
                    $out .= ",\n\t" . $atr_name . " INT";
                }
            }
        }

        foreach ($relations as $parent => $child) {
            $parent = mb_strtolower($parent, 'UTF-8');
            $parent_id = $parent . "_id";

            foreach ($relations[$parent] as $key_child => $attribute) {
                if (array_key_exists("etc", $parameters) && $parameters["etc"] < $attribute && $key_child == $key) {
                    if (array_key_exists($parent_id, $value["keys"]) || array_key_exists($parent_id, $value["attributes"]))
                        panic("Conflict of keys", 90);
                    $out .= ",\n\t" . $parent_id . " INT";
                }
            }
        }


        if (array_key_exists("a", $parameters)) {
            if (isset($value["value"]))
                $out .= ",\n\t" . "value " . data_type_to_string($value["value"]);
        } else {

            foreach ($value["attributes"] as $key_attr => $attribute) {
                if ($key_attr != "value")
                    $out .= ",\n\t" . $key_attr . " " . data_type_to_string($attribute);
                else
                    $value["value"] = data_type_priority($value["value"], $attribute);
            }

            if (isset($value["value"]))
                $out .= ",\n\t" . "value " . data_type_to_string($value["value"]);
        }

        $out .= "\n);\n\n";
    }

    return $out;
}


/**
 * Function generates table of relationships
 *
 * @param $tables array tables
 * @param $relations array of relations
 * @param $parameters array program parameeters
 * @return string
 */
function relationships_table($tables, $relations, $parameters)
{
    $relations_array = array();
    foreach ($tables as $key => $value) {
        foreach ($relations[$key] as $key_attr => $attribute) {
            if ((!array_key_exists("etc", $parameters) && !array_key_exists("b", $parameters)) || (array_key_exists("etc", $parameters) && $parameters["etc"] >= $attribute) || array_key_exists("b", $parameters))
                $relations_array[$key][$key_attr] = 1;
        }

        foreach ($relations as $parent => $child) {
            foreach ($relations[$parent] as $key_child => $attribute) {
                if (array_key_exists("etc", $parameters) && ($parameters["etc"] < $attribute) && $key_child == $key)
                    $relations_array[$key][$parent] = 1;
            }
        }

        $relations_array[$key][$key] = "1:1";
    }

    foreach ($relations_array as $key_parent => $value) {
        foreach ($relations_array[$key_parent] as $key_child => $value_child) {
            $relations_array[$key_parent][$key_child] = get_relation($relations_array, $key_parent, $key_child);
        }
    }

    foreach ($relations_array as $key_parent => $value) {
        foreach ($relations_array[$key_parent] as $key_child => $value_child) {
            if ($relations_array[$key_parent][$key_child] == "1:N" || $relations_array[$key_parent][$key_child] == "N:1")
                $relations_array[$key_child][$key_parent] = ($relations_array[$key_parent][$key_child] == "1:N" ? "N:1" : "1:N");

            if ($relations_array[$key_parent][$key_child] == "N:M")
                $relations_array[$key_child][$key_parent] = "N:M";
        }
    }

    $change = true;
    while ($change) {
        $change = false;
        foreach ($relations_array as $a => $value) {
            foreach ($relations_array[$a] as $c => $value_child) {
                if ($relations_array[$a][$c] == "1:N" || $relations_array[$a][$c] == "N:1") {
                    foreach ($relations_array[$c] as $b => $value) {
                        if ($relations_array[$c][$b] == $relations_array[$a][$c] && !isset($relations_array[$a][$b])) {
                            $change = true;
                            $relations_array[$a][$b] = $relations_array[$c][$b];
                        }
                    }
                }
            }
        }
    }

    $change = true;
    while ($change) {
        $change = false;
        foreach ($relations_array as $a => $value) {
            foreach ($relations_array[$a] as $c => $value_child) {
                foreach ($relations_array[$c] as $b => $value) {
                    if (!isset($relations_array[$a][$b])) {
                        $change = true;
                        $relations_array[$a][$b] = $relations_array[$b][$a] = "N:M";
                    }
                }
            }
        }
    }

    return get_relations_xml($relations_array);
}


/**
 * Generates the XML of relationships
 *
 * @param $relations array of relationships
 * @return string xml relations
 */
function get_relations_xml($relations)
{
    $out = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    $out .= "<tables>\n";

    foreach ($relations as $key => $value) {
        $out .= "\t<table name=\"" . $key . "\">\n";
        foreach ($value as $key_child => $relation) {
            $out .= "\t\t<relation to=\"" . $key_child . "\" relation_type=\"" . $relation . "\" />\n";
        }
        $out .= "\t\t</table>\n";
    }
    $out .= "</tables>\n";

    return $out;
}

/**
 * Function returns relationship type - 1:1, 1:N, N:1, N:M
 *
 * @param $relations_array
 * @param $parent
 * @param $child
 * @return string
 */
function get_relation($relations_array, $parent, $child)
{
    if ($parent == $child)
        return "1:1";

    $i1 = array_key_exists($child, $relations_array[$parent]) ? "N" : "1";

    $i2 = array_key_exists($parent, $relations_array[$child]) ? "N" : "1";

    if ($i1 == "N" && $i2 == "N") {
        $i2 = "M";
    }

    return $i1 . ":" . $i2;
}


/**
 * Function returns higher priority datatype
 * @param $dt1 int type 1
 * @param $dt2 int type 2
 * @return int higher priority data type
 */
function data_type_priority($dt1, $dt2)
{
    return $dt1 > $dt2 ? $dt1 : $dt2;
}

/**
 * Function converts datatype from number representation to string
 * @param $data_type
 * @return null|string string representation of data type
 */
function data_type_to_string($data_type)
{
    switch ($data_type) {
        case 0:
            return "BIT";
        case 1:
            return "INT";
        case 2:
            return "FLOAT";
        case 3:
            return "NVARCHAR";
        case 4:
            return "NTEXT";
    }

    return NULL;
}

/**
 * Function converts xml object to array
 *
 * @param $xml object xml object to convert
 * @return array xml in associative array representation
 */
function xml_to_array($xml)
{
    $objects_array = array();

    foreach ($xml->attributes() as $key => $value) {
        $objects_array['@attr'][$key] = (string)$value;
    }

    foreach ($xml as $key => $value) {
        $objects_array[$key][] = xml_to_array($value);
    }

    if (!$xml->count())
        $objects_array['@val'] = (string)$xml;

    return $objects_array;
}

/**
 * Function returns data type of value in string.
 *
 * @param $string string to determine data type
 * @return int data type {0 - bool, 1 - int, 2 - float, 4 - ntext}
 */
function get_data_type($string)
{
    $string = mb_strtolower(trim($string), 'UTF-8');

    if (empty($string))
        return NULL;


    if (preg_match('/^(\s+|true|false|0|1)$/', $string))
        return 0;
    if (preg_match('/^-?\d+$/', $string))
        return 1;
    if (preg_match('/^-?\d+.\d+(e[-|\+]?\d+)?$/', $string))
        return 2;

    return 4;
}


/**
 * Parse CLI options
 *
 * @param $argv array program arguments
 *
 * @return array of options
 */
function handle_parameters($argv)
{
    unset($argv[0]);


    foreach ($argv as $arg) {
        if (!preg_match('/^(--((isvalid)|(input)|(output)|(header))=.+)|(--etc=\d+)|(^-[abg]+)|(^--help)$/', $arg)) {
            panic($arg . " is not valid option.", 1);
        }
    }

    $short_opts = "a";
    $short_opts .= "b";
    $short_opts .= "g";

    $long_opts = array("help",
        "input::",
        "output::",
        "header::",
        "etc::",
        "isvalid::",
    );

    $options = getopt($short_opts, $long_opts);
    if (isset($options["etc"])) {
        $options["etc"] = intval($options["etc"]);
    }

    if (isset($options["etc"]) && isset($options["b"])) {
        panic("Invalid combination of -b and --etc= switches", 1);
    }

    if (isset($options["help"])) {
        if (!preg_match('/^--help$/', implode(" ", $argv)))
            panic("Option --help cannot be combined with other options", 1);
        else {
            print_help();
            exit(0);
        }
    }

    if (isset($options["output"])) {
        if ($options["output"] === $options["input"])
            panic("Output file cannot be same as input");
    }

    return $options;
}


/**
 * Prints help to the STDOUT and exits with error code.
 */
function print_help()
{
    printf("
	Help:\n
	--help
	--input=FILENAME   - input file
	--output=FILENAME  - output file
	--header='HEADER'  - commented output file header 
	--etc=N            - max number of columns generated from subelements of same name
	-a - columns won't be generated from attributes of input xml elements
	-b - more subelements of same name will be counted as 1
	   - cannot be combined with --etc option
	-g - relationship table in format xml will be printed\n");
}


/**
 * Function writes error message to STDERR and exits program
 *
 * @param string $msg Error message
 * @param int $code Exit code
 */
function panic($msg = "", $code = -1)
{
    fprintf(STDERR, "%s\n", $msg);
    exit($code);
}

?>