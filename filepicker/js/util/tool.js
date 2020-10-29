/* eslint-disable no-unused-vars */

function getSizeStr (bytes) {
  var unit = ['B', 'KB', 'MB', 'GB', 'TB']
  var i = 0
  if (bytes < 0) {
    console.log('getSizeStr() bytes wrong: ', bytes)
    return '0'
  }

  while (bytes > 1024 && i < unit.length) {
    bytes /= 1024.0
    i++
    // str = Number(bytes).toPrecision(4) + unit[i]
  }

  var precision = bytes >= 1000 ? 4 : 3
  return Number(bytes).toPrecision(precision) + unit[i]
}

function getDir (filePath) {
  if (!filePath) {
    return ''
  }
  return filePath.substr(0, filePath.lastIndexOf('/'))
}


var _UI_RATIO = 1;
var UI = {
  /* all */
  DEFAULT_MARGIN_LEFT: 40 * _UI_RATIO,
  DEFAULT_MARGIN_RIGHT: 40 * _UI_RATIO,

  /* title bar */
  TITLEBAR_HEIGHT: 106 * _UI_RATIO,

  /* file list */
  FILE_LIST_HEIGHT: 160 * _UI_RATIO,
  FILE_LIST_ROW_SPACING: 30 * _UI_RATIO,
  FILE_LIST_COLUMN_SPACING: 10 * _UI_RATIO,
  FILE_ICON_WIDTH: 82 * _UI_RATIO,
  FILE_ICON_HEIGHT: 82 * _UI_RATIO,
  FILE_NAME_SIZE: 32 * _UI_RATIO,
  FILE_INFO_SIZE: 26 * _UI_RATIO,
  FILE_INFO_COLOR: '#8d8d8d'
}
