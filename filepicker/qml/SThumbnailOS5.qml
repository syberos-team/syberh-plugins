import QtQuick 2.0
import Nemo.Thumbnailer 1.0
import "../js/util/tool.js" as Tool

Thumbnail {
    property string filePath

    width: Tool.UI.FILE_ICON_WIDTH
    height: Tool.UI.FILE_ICON_HEIGHT
    visible: (fileUtils.getFileType(filePath) === "image" || fileUtils.getFileType(filePath) === "video") ? true : false
    clip: true

    sourceSize.width: Tool.UI.FILE_ICON_WIDTH
    sourceSize.height: Tool.UI.FILE_ICON_HEIGHT
    source: "file://" + encodeURIComponent(filePath)
    mimeType : fileUtils.getFileType(filePath)
}