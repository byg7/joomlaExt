<?php
// загружаем ядро
define('_JEXEC', 1);
define('DS', DIRECTORY_SEPARATOR);
if (file_exists(dirname(__FILE__) . '/../../defines.php')) {
	include_once dirname(__FILE__) . '/../../defines.php';
}
if (!defined('_JDEFINES')) {
	define('JPATH_BASE', dirname(__FILE__) . '/../..');
	require_once (JPATH_BASE.'/includes/defines.php');
	require_once (JPATH_BASE.'/includes/framework.php');
}
require_once JPATH_BASE.'/includes/framework.php';
$app = JFactory::getApplication('site');
$app->initialise();
$user = JFactory::getUser();
include(__DIR__.'/out/kernel.php');
