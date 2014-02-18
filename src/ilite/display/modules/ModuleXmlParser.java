package ilite.display.modules;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class ModuleXmlParser
{
   private final String mFilePath;
   private static final String sFileName = "modules.xml";
   private List<Module> mModules = new ArrayList<Module>();

   public ModuleXmlParser(String pFilePath)
   {
      mFilePath = pFilePath;
   }
   
   public String getFileName()
   {
	   return "" + mFilePath + sFileName;
   }

   public void parse() throws ParserConfigurationException, SAXException,
         IOException
   {
      Document doc = null;
      File file = null;
      if (mFilePath == null)
         file = new File(sFileName);
      else
         file = new File(mFilePath, sFileName);

      DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
      DocumentBuilder db = dbf.newDocumentBuilder();
      doc = db.parse(file);
      doc.getDocumentElement().normalize();
      NodeList nodeLst = doc.getElementsByTagName("module");

      for (int s = 0; s < nodeLst.getLength(); s++)
      {

         Node fstNode = nodeLst.item(s);

         if (fstNode.getNodeType() == Node.ELEMENT_NODE)
         {

            Element fstElmnt = (Element) fstNode;
            String control = null;

            NodeList lstNmElmntLst = fstElmnt.getElementsByTagName("control");
            Element lstNmElmnt = (Element) lstNmElmntLst.item(0);
            NodeList lstNm = lstNmElmnt.getChildNodes();
            control = ((Node) lstNm.item(0)).getNodeValue();

            Module m = null;
            m = new Module(control);
            mModules.add(m);
         }

      }

   }

   public List<Module> getModules()
   {
      return new ArrayList<Module>(mModules);
   }

}
