#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>
#include <poppler/glib/poppler.h>
#include <cairo.h>
#include <poppler/Page.h>
#include <poppler/PDFDoc.h>
#include <poppler/TextOutputDev.h>
//#include <OutputDev.h>
//#include <Stream.h>
#include <gdk/gdk.h>

struct _PopplerDocument
{
    /*< private >*/
    GObject parent_instance;
    PDFDoc *doc;

    GList *layers;
    GList *layers_rbgroups;
    OutputDev *output_dev;
};

struct _PopplerPage
{
    /*< private >*/
    GObject parent_instance;
    PopplerDocument *document;
    Page *page;
    int index;
    TextPage *text;
    Annots *annots;
};

namespace node {
    class NodePopplerPage : public ObjectWrap {
	public:
	    NodePopplerPage(NodePopplerDocument* doc, int32_t index);
	    ~NodePopplerPage();
	    static void Initialize(v8::Handle<v8::Object> target);
        GdkPixbuf *render_to_pixbuf(int width, int height, double scale);
        GdkPixbuf *cairo_to_pixbuf(cairo_surface_t *surface);
        bool isOk() {
            return page != NULL;
        }
	protected:
	    static v8::Handle<v8::Value> New(const v8::Arguments &args);
	    static v8::Handle<v8::Value> render(const v8::Arguments &args);
        static v8::Handle<v8::Value> findText(const v8::Arguments &args);

	private:
	    static v8::Persistent<v8::FunctionTemplate> constructor_template;
        static void _render(uv_work_t *work_req);
        static void afterRender(uv_work_t *work_req);

	    static v8::Handle<v8::Value> paramsGetter(v8::Local<v8::String> property, const v8::AccessorInfo &info);

	    PopplerDocument* document;
	    PopplerPage* page;
	    double width, height;
        GList *mapping;

        PDFDoc doc;
        Page pg;

	    friend class NodePopplerDocument;
    };
}
